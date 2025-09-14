// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Settings/Project.hpp"
#include <fstream>
#include <Pbox/Logger.hpp>
#include <Pbox/Settings/Detail/TypesSerialization.hpp>
#include "CameraLed.hpp"
#include "CollageCaptureSession.hpp"
#include "CollageContext.hpp"
#include "EspHomeClient.hpp"
#include "EspHomeRemoteTrigger.hpp"
#include "ICaptureSession.hpp"
#include "RemoteTrigger.hpp"
#include "SingleCaptureSession.hpp"

DEFINE_LOGGER(project);

namespace Pbox
{
class CaptureSessionFactory
{
  public:
    virtual CaptureSessionPtr create() = 0;
    virtual ~CaptureSessionFactory();
};

class SingleCaptureSessionFactory : public CaptureSessionFactory
{
  public:
    CaptureSessionPtr create() override
    {
        return make_unique_object_ptr_as<ICaptureSession, SingleCaptureSession>();
    }
};

class CollageCaptureSessionFactory : public CaptureSessionFactory
{
    CollageContext collage_context_;

  public:
    CaptureSessionPtr create() override
    {
        return make_unique_object_ptr_as<ICaptureSession, CollageCaptureSession>(collage_context_);
    }
};

AbstractCaptureSessionFactory::~AbstractCaptureSessionFactory() = default;

CaptureSessionPtr AbstractCaptureSessionFactory::createFromTriggerCondition(const RemoteTriggerId &trigger_id)
{
    const auto it = factories_.find(trigger_id);
    if (it == factories_.end())
    {
        LOG_ERROR(logger_project(), "Could not find a session factory for the trigger '{}'", trigger_id);
        return nullptr;
    }
    return it->second->create();
}

class DefaultRemoteTriggerFactory final : public IRemoteTriggerFactory
{
  public:
    std::unique_ptr<RemoteTrigger> create(const EspHomeRemoteTriggerConfig &config) override
    {
        return std::make_unique<EspHomeRemoteTrigger>(
            QString::fromStdString(config.name),
            std::make_unique<EspHomeClient>(QUrl{QString::fromStdString(config.uri)}));
    }
};
std::unique_ptr<IRemoteTriggerFactory> createDefaultRemoteTriggerFactory()
{
    return std::make_unique<DefaultRemoteTriggerFactory>();
}

Project::Project(Instance<TriggerManager> trigger_manager,
                 std::unique_ptr<IRemoteTriggerFactory> remote_trigger_factory)
    : trigger_manager_{std::move(trigger_manager)}
    , remote_trigger_factory_{std::move(remote_trigger_factory)}
{}

void Project::initFromConfig(const std::filesystem::path &config_file)
{
    std::ifstream settings_file{config_file};
    nlohmann::json json;
    settings_file >> json;

    ProjectConfig settings;
    json.get_to(settings);

    name_ = settings.name;

    for (auto &&trigger : settings.remote_triggers)
    {
        trigger_manager_->registerTrigger(trigger.name, remote_trigger_factory_->create(trigger));
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
