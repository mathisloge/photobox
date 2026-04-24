// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <Pbox/DisableCopyMove.hpp>
#include "CaptureSessionManager.hpp"
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

/**
 * @brief Creates the default remote trigger factory.
 * @return A unique pointer to the factory.
 */
std::unique_ptr<IRemoteTriggerFactory> createDefaultRemoteTriggerFactory();

/**
 * @brief Main configuration container for the photobox application.
 *
 * This class loads and manages the entire application configuration
 * from a config file, initializing all components and dependencies.
 */
class Project
{
  public:
    PBOX_DISABLE_COPY_MOVE(Project);
    /**
     * @brief Constructs a Project with the given dependencies.
     * @param trigger_manager The trigger manager.
     * @param capture_session_manager The capture session manager.
     * @param image_storage The image storage.
     * @param scheduler The scheduler.
     * @param font_cache The font cache.
     * @param remote_trigger_factory The remote trigger factory.
     */
    explicit Project(
        Instance<TriggerManager> trigger_manager,
        Instance<CaptureSessionManager> capture_session_manager,
        Instance<ImageStorage> image_storage,
        Instance<Scheduler> scheduler,
        Instance<SvgFontCache> font_cache,
        std::unique_ptr<IRemoteTriggerFactory> remote_trigger_factory = createDefaultRemoteTriggerFactory());
    ~Project();

    /**
     * @brief Initializes the project from a config file.
     * @param config_file The path to the config file.
     */
    void initFromConfig(const std::filesystem::path &config_file);

    /**
     * @brief Gets the project name.
     * @return The project name.
     */
    const std::string &name() const;
    /**
     * @brief Gets the camera LED controller.
     * @return Reference to the camera LED.
     */
    CameraLed &cameraLed();

  private:
    Instance<TriggerManager> trigger_manager_;                      /**< Trigger manager. */
    Instance<CaptureSessionManager> capture_session_manager_;       /**< Capture session manager. */
    Instance<ImageStorage> image_storage_;                          /**< Image storage. */
    Instance<Scheduler> scheduler_;                                 /**< Scheduler. */
    Instance<SvgFontCache> font_cache_;                             /**< Font cache. */
    std::unique_ptr<IRemoteTriggerFactory> remote_trigger_factory_; /**< Remote trigger factory. */
    std::string name_;                                              /**< Project name. */
    std::filesystem::path output_directory_;                        /**< Output directory. */
    std::unique_ptr<CameraLed> camera_led_;                         /**< Camera LED controller. */
};
} // namespace Pbox
