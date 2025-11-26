// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <QQuickStyle>
#include <QWindow>
#include <ApplicationState.hpp>
#include <CrashCollection.hpp>
#include <EspHomeCameraLed.hpp>
#include <EspHomeClient.hpp>
#include <EspHomeRemoteTrigger.hpp>
#include <ICamera.hpp>
#include <ImageStorage.hpp>
#include <MockCamera.hpp>
#include <Pbox/GPhoto2Camera.hpp>
#include <Pbox/Logger.hpp>
#include <Pbox/Settings/Project.hpp>
#include <Pbox/SetupLogging.hpp>
#include <Scheduler.hpp>
#include <TriggerManager.hpp>
#include <fmt/core.h>
#include "CaptureManager.hpp"
#include "SvgFontCache.hpp"
#include "SystemStatusManager.hpp"

Q_IMPORT_QML_PLUGIN(Photobox_CorePlugin)
Q_IMPORT_QML_PLUGIN(Photobox_UiPlugin)
Q_IMPORT_QML_PLUGIN(Photobox_SettingsPlugin)

DEFINE_ROOT_LOGGER(root)

using namespace Pbox;

int main(int argc, char *argv[])
{
    const auto crash_result = install_crash_handler();
    setupLogging();
    if (not crash_result.has_value())
    {

        LOG_CRITICAL(logger_root(), "Could not install crash handler. Failed with: {}", crash_result.error().message());
        exit(EXIT_FAILURE);
    }
    installQtMessageHandler();
    int app_return_code{EXIT_FAILURE};
    Instance<Scheduler> scheduler = std::make_shared<Scheduler>();

    {
        QApplication app(argc, argv);
        QCoreApplication::setApplicationName(QStringLiteral("PhotoBox"));
        QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge.photobox"));
        QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));
        QQuickStyle::setStyle("FluentWinUI3");

        QCommandLineParser parser;
        parser.addHelpOption();
        parser.addVersionOption();

        QCommandLineOption config_option(QStringList{"config"}, "Configuration json path", "path", "./photobox.json");
        parser.addOption(config_option);

        QCommandLineOption developer_option(QStringList{"dev"}, "Use developer mode");
        parser.addOption(developer_option);

        QCommandLineOption camera_led_host_option(
            QStringList() << "camera-led", "The hostname of the camera led controller", "url", "http://192.168.178.31");
        parser.addOption(camera_led_host_option);

        QCommandLineOption fullscreen_option{QStringList{QStringLiteral("f"), QStringLiteral("fullscreen")},
                                             QStringLiteral("shows the application in fullscreen mode")};
        parser.addOption(fullscreen_option);

        parser.process(app);

        const bool developer_mode = parser.isSet(developer_option);
        const QString camera_led_host = parser.value(camera_led_host_option);
        const QWindow::Visibility window_mode =
            parser.isSet(fullscreen_option) ? QWindow::Visibility::FullScreen : QWindow::Visibility::Windowed;

        LOG_NOTICE(logger_root(), "developer_mode={}", developer_mode);
        LOG_NOTICE(logger_root(), "camera_led_host={}", camera_led_host.toStdString());
        LOG_NOTICE(logger_root(), "window_mode={}", static_cast<int>(window_mode));

        Instance<ImageStorage> image_storage = std::make_shared<ImageStorage>("photobox-capture");
        Instance<SystemStatusManager> system_status_manager = std::make_shared<SystemStatusManager>();
        Instance<TriggerManager> trigger_manager = std::make_shared<TriggerManager>(system_status_manager);
        Instance<CaptureSessionManager> capture_session_manager = std::make_shared<CaptureSessionManager>();
        Instance<SvgFontCache> svg_font_cache = std::make_shared<SvgFontCache>();
        Project project{trigger_manager, capture_session_manager, image_storage, scheduler, svg_font_cache};

        try
        {
            project.initFromConfig(parser.value(config_option).toStdString());
        }
        catch (const std::exception &ex)
        {
            LOG_CRITICAL(logger_root(),
                         "Could not load configuration file from path {}. Error: {}",
                         parser.value(config_option).toStdString(),
                         ex.what());
            return EXIT_FAILURE;
        }

        Instance<CameraLed> camera_led =
            std::make_shared<EspHomeCameraLed>(std::make_unique<EspHomeClient>(camera_led_host));
        std::shared_ptr<ICamera> camera;

        if (not developer_mode)
        {
            camera = std::make_shared<GPhoto2Camera>(scheduler);
        }
        else
        {
            camera = std::make_shared<MockCamera>();
        }

        Instance<CaptureManager> capture_manager = std::make_shared<CaptureManager>(
            scheduler, image_storage, camera, trigger_manager, camera_led, capture_session_manager);
        system_status_manager->registerClient(camera->systemStatusClient());

        QQmlApplicationEngine engine;

        engine.setInitialProperties({
            {"visibility", window_mode},
        });
        auto &&app_state = engine.singletonInstance<ApplicationState *>("Photobox.Core", "ApplicationState");
        Q_ASSERT(app_state != nullptr);

        app_state->system_status_manager = system_status_manager;
        app_state->capture_manager = capture_manager;

        engine.loadFromModule("Photobox.App", "Main");
        engine.addImageProvider(QLatin1String("preview-image"), capture_manager->createImageProvider());

        app_return_code = app.exec();
    }
    return app_return_code;
}
