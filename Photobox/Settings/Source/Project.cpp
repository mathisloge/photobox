// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Settings/Project.hpp"
#include <fstream>
#include <Pbox/Logger.hpp>
#include <Pbox/Sessions/CollageCaptureSessionFactory.hpp>
#include <Pbox/Sessions/SingleCaptureSessionFactory.hpp>
#include <Pbox/Settings/Detail/TypesSerialization.hpp>
#include "CameraLed.hpp"
#include "EspHomeClient.hpp"
#include "EspHomeRemoteTrigger.hpp"
#include "RemoteTrigger.hpp"

DEFINE_LOGGER(project);

namespace Pbox
{

namespace
{
std::filesystem::path absolute_from_settings_root(const std::filesystem::path &root, const std::filesystem::path &path)
{
    if (path.is_absolute())
    {
        return path;
    }
    return root / path;
}
} // namespace

class DefaultRemoteTriggerFactory final : public IRemoteTriggerFactory
{
  public:
    std::unique_ptr<RemoteTrigger> createEsphomeTrigger(RemoteTriggerId name, std::string uri) override
    {
        return std::make_unique<EspHomeRemoteTrigger>(
            QString::fromStdString(std::move(name)),
            std::make_unique<EspHomeClient>(QUrl{QString::fromStdString(uri)}));
    }
};
std::unique_ptr<IRemoteTriggerFactory> createDefaultRemoteTriggerFactory()
{
    return std::make_unique<DefaultRemoteTriggerFactory>();
}

Project::Project(Instance<TriggerManager> trigger_manager,
                 Instance<CaptureSessionManager> capture_session_manager,
                 Instance<ImageStorage> image_storage,
                 Instance<Scheduler> scheduler,
                 Instance<SvgFontCache> font_cache,
                 std::unique_ptr<IRemoteTriggerFactory> remote_trigger_factory)
    : trigger_manager_{std::move(trigger_manager)}
    , capture_session_manager_{std::move(capture_session_manager)}
    , image_storage_{std::move(image_storage)}
    , scheduler_{std::move(scheduler)}
    , font_cache_{std::move(font_cache)}
    , remote_trigger_factory_{std::move(remote_trigger_factory)}
{}

void Project::initFromConfig(const std::filesystem::path &config_file)
{
    std::ifstream settings_file{config_file};
    nlohmann::json json;
    settings_file >> json;

    const std::filesystem::path root_path{config_file.parent_path()};
    LOG_DEBUG(logger_project(), "using root dir: {}", root_path.string());

    ProjectConfig settings;
    try
    {
        json.get_to(settings);
    }
    catch (const std::exception &ex)
    {
        LOG_ERROR(logger_project(),
                  "Could not load project settings file from '{}'. Error: {}",
                  config_file.string(),
                  ex.what());
    }

    name_ = settings.name;
    image_storage_->updateStorageDir(absolute_from_settings_root(root_path, settings.capture_dir));
    capture_session_manager_->setInitialCountdown(settings.initial_countdown);

    for (auto &&font : settings.fonts)
    {
        font_cache_->registerFont(
            font.family, font.bold, font.italic, absolute_from_settings_root(root_path, font.path));
    }

    for (auto &&trigger : std::as_const(settings.remote_triggers))
    {
        trigger_manager_->registerTrigger(trigger.name,
                                          remote_trigger_factory_->createEsphomeTrigger(trigger.name, trigger.uri));
    }

    for (auto &&session : settings.sessions)
    {
        if (session.collage.has_value())
        {
            session.collage->svg_template = absolute_from_settings_root(root_path, session.collage->svg_template);
        }
        switch (session.type)
        {
        case SessionType::SingleCapture:
            capture_session_manager_->registerCaptureSession(
                session.name, std::make_unique<SingleCaptureSessionFactory>(session.name));
            break;
        case SessionType::CollageCapture:
            capture_session_manager_->registerCaptureSession(
                session.name,
                std::make_unique<CollageCaptureSessionFactory>(
                    session.name, scheduler_, image_storage_, session.collage.value_or(CollageSettings{})));
            break;
        case SessionType::Unknown:
            LOG_WARNING(logger_project(), "Got unknown session type for session '{}'", session.name);
            break;
        }

        for (auto &&trigger : session.triggers)
        {
            capture_session_manager_->addTriggerRelation(session.name, trigger);
        }
    }
}

const std::string &Project::name() const
{
    return name_;
}

CameraLed &Project::cameraLed()
{
    return *camera_led_;
}

Project::~Project() = default;

} // namespace Pbox
