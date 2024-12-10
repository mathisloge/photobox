#include "Project.hpp"
#include <Pbox/Logger.hpp>
#include "CameraLed.hpp"
#include "CollageCaptureSession.hpp"
#include "CollageContext.hpp"
#include "ICaptureSession.hpp"
#include "ProjectSettings.hpp"
#include "RemoteTrigger.hpp"
#include "SingleCaptureSession.hpp"

DEFINE_LOGGER(abstract_caputure_session_factory);

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

Project::Project(const std::filesystem::path &config_file)
{}

Project::~Project() = default;

} // namespace Pbox
