// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <Pbox/DisableCopyMove.hpp>
#include "CaptureSessionFactory.hpp"
#include "IRemoteTriggerFactory.hpp"
#include "ImageStorage.hpp"
#include "Scheduler.hpp"
#include "SvgFontCache.hpp"
#include "TriggerManager.hpp"
#include "Types.hpp"

namespace Pbox
{
class CameraLed;
class RemoteTrigger;

std::unique_ptr<IRemoteTriggerFactory> createDefaultRemoteTriggerFactory();

class Project
{
  public:
    PBOX_DISABLE_COPY_MOVE(Project);
    explicit Project(
        Instance<TriggerManager> trigger_manager,
        Instance<CaptureSessionManager> capture_session_manager,
        Instance<ImageStorage> image_storage,
        Instance<Scheduler> scheduler,
        Instance<SvgFontCache> font_cache,
        std::unique_ptr<IRemoteTriggerFactory> remote_trigger_factory = createDefaultRemoteTriggerFactory());
    ~Project();

    void initFromConfig(const std::filesystem::path &config_file);

    const std::string &name() const;
    CameraLed &cameraLed();

  private:
    Instance<TriggerManager> trigger_manager_;
    Instance<CaptureSessionManager> capture_session_manager_;
    Instance<ImageStorage> image_storage_;
    Instance<Scheduler> scheduler_;
    Instance<SvgFontCache> font_cache_;
    std::unique_ptr<IRemoteTriggerFactory> remote_trigger_factory_;
    std::string name_;
    std::filesystem::path output_directory_;
    std::unique_ptr<CameraLed> camera_led_;
};
} // namespace Pbox
