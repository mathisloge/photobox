#pragma once
#include <filesystem>
#include <unordered_map>
#include <Pbox/DisableCopyMove.hpp>
#include "ProjectSettings.hpp"

namespace Pbox
{
class CameraLed;
class RemoteTrigger;
class ICaptureSession;
using CaptureSessionPtr = std::unique_ptr<ICaptureSession>;

class CaptureSessionFactory;
class AbstractCaptureSessionFactory
{
  public:
    ~AbstractCaptureSessionFactory();
    CaptureSessionPtr createFromTriggerCondition(const RemoteTriggerId &trigger_id);

  private:
    std::unordered_map<RemoteTriggerId, std::unique_ptr<CaptureSessionFactory>> factories_;
};

class Project
{
  public:
    PBOX_DISABLE_COPY_MOVE(Project);
    explicit Project(const std::filesystem::path &config_file);
    ~Project();

  private:
    std::filesystem::path output_directory_;
    std::unique_ptr<CameraLed> camera_led_;
    std::unordered_map<RemoteTriggerId, std::unique_ptr<RemoteTrigger>> remote_triggers_;
};
} // namespace Pbox
