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

DEFINE_LOGGER(abstract_caputure_session_factory);
DEFINE_LOGGER(log_project);

namespace Pbox
{
class CaptureSessionFactory
{
  public:
    virtual std::unique_ptr<ICaptureSession> create() = 0;
    virtual ~CaptureSessionFactory();
};

class SingleCaptureSessionFactory : public CaptureSessionFactory
{
  public:
    std::unique_ptr<ICaptureSession> create() override
    {
        return std::make_unique<SingleCaptureSession>();
    }
};

class CollageCaptureSessionFactory : public CaptureSessionFactory
{
    CollageContext collage_context_;

  public:
    std::unique_ptr<ICaptureSession> create() override
    {
        return std::make_unique<CollageCaptureSession>(collage_context_);
    }
};

AbstractCaptureSessionFactory::~AbstractCaptureSessionFactory() = default;

class DefaultRemoteTriggerFactory final : public IRemoteTriggerFactory
{
  public:
    std::unique_ptr<RemoteTrigger> create(const EspHomeRemoteTriggerConfig &config) override
    {
        return std::make_unique<EspHomeRemoteTrigger>(
            std::make_unique<EspHomeClient>(QUrl{QString::fromStdString(config.uri)}));
    }
};
std::unique_ptr<IRemoteTriggerFactory> createDefaultRemoteTriggerFactory()
{
    return std::make_unique<DefaultRemoteTriggerFactory>();
}

CaptureSessionPtr AbstractCaptureSessionFactory::createFromTriggerCondition(const RemoteTriggerId &trigger_id)
{
    const auto it = factories_.find(trigger_id);
    if (it == factories_.end())
    {
        LOG_ERROR(
            abstract_caputure_session_factory, "Could not find a session factory for the trigger '{}'", trigger_id);
        return nullptr;
    }
    return it->second->create();
}

Project::Project(const std::filesystem::path &config_file, std::unique_ptr<IRemoteTriggerFactory> remoteTriggerFactory)
{
    std::ifstream settings_file{config_file};
    nlohmann::json json;
    settings_file >> json;

    ProjectConfig settings;
    json.get_to(settings);

    name_ = settings.name;

    for (auto &&trigger : settings.remote_triggers)
    {
        LOG_DEBUG(log_project, "Creating remote trigger '{}'", trigger.name);
        remote_triggers_.emplace(trigger.name, remoteTriggerFactory->create(trigger));
    }
}

const std::string &Project::name() const
{
    return name_;
}

Project::~Project() = default;

} // namespace Pbox
