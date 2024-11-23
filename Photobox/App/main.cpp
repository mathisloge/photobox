#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <QWindow>
#include <ApplicationState.hpp>
#include <CameraImageProvider.hpp>
#include <CaptureController.hpp>
#include <CollagePrinter.hpp>
#include <CrashCollection.hpp>
#include <EspHomeCameraLed.hpp>
#include <EspHomeClient.hpp>
#include <EspHomeRemoteTrigger.hpp>
#include <GPhoto2Camera.hpp>
#include <ICamera.hpp>
#include <ImageStorage.hpp>
#include <MockCamera.hpp>
#include <Pbox/Logger.hpp>
#include <Pbox/SetupLogging.hpp>
#include <Scheduler.hpp>
#include <fmt/core.h>
#include "CaptureManager.hpp"
#include "CollageContext.hpp"
#include "SystemStatusManager.hpp"

Q_IMPORT_QML_PLUGIN(Photobox_CorePlugin)
Q_IMPORT_QML_PLUGIN(Photobox_UiPlugin)

DEFINE_ROOT_LOGGER(rootlogger)

using namespace Pbox;

int main(int argc, char *argv[])
{
    install_crash_handler();
    setupLogging();
    int app_return_code{EXIT_FAILURE};
    Scheduler scheduler;

    {
        QApplication app(argc, argv);
        QCoreApplication::setApplicationName(QStringLiteral("PhotoBox"));
        QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge.photobox"));
        QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

        QCommandLineParser parser;
        parser.addHelpOption();
        parser.addVersionOption();
        QCommandLineOption capture_directory_option(
            QStringList() << "storage-dir", "the captured image storage dir", "path to dir", "captures");
        parser.addOption(capture_directory_option);

        QCommandLineOption collage_directory_option(
            QStringList() << "collage-dir", "The dir with the collage settings", "path to dir", "collage");
        parser.addOption(collage_directory_option);

        QCommandLineOption printer_settings_option(QStringList() << "printer-settings",
                                                   "The printer_settings.json file",
                                                   "path to file",
                                                   "printer_settings.json");
        parser.addOption(printer_settings_option);

        QCommandLineOption developer_option(QStringList{"dev"}, "Use developer mode");
        parser.addOption(developer_option);

        QCommandLineOption trigger_button_host_option(
            QStringList() << "trigger-btn", "The hostname of the trigger button", "url", "http://192.168.0.31");
        parser.addOption(trigger_button_host_option);

        QCommandLineOption camera_led_host_option(
            QStringList() << "camera-led", "The hostname of the camera led controller", "url", "http://192.168.0.31");
        parser.addOption(camera_led_host_option);

        QCommandLineOption fullscreen_option{QStringList{QStringLiteral("f"), QStringLiteral("fullscreen")},
                                             QStringLiteral("shows the application in fullscreen mode")};
        parser.addOption(fullscreen_option);

        parser.process(app);

        const QString capture_directory = parser.value(capture_directory_option);
        const QString collage_directory = parser.value(collage_directory_option);
        const QString printer_settings = parser.value(printer_settings_option);
        const bool developer_mode = true or parser.isSet(developer_option);
        const QString trigger_button_host = parser.value(trigger_button_host_option);
        const QString camera_led_host = parser.value(camera_led_host_option);
        const QWindow::Visibility window_mode =
            parser.isSet(fullscreen_option) ? QWindow::Visibility::FullScreen : QWindow::Visibility::Windowed;

        LOG_NOTICE(rootlogger, "capture_directory={}", capture_directory.toStdString());
        LOG_NOTICE(rootlogger, "collage_directory={}", collage_directory.toStdString());
        LOG_NOTICE(rootlogger, "printer_settings={}", printer_settings.toStdString());
        LOG_NOTICE(rootlogger, "developer_mode={}", developer_mode);
        LOG_NOTICE(rootlogger, "trigger_button_host={}", trigger_button_host.toStdString());
        LOG_NOTICE(rootlogger, "camera_led_host={}", camera_led_host.toStdString());
        LOG_NOTICE(rootlogger, "window_mode={}", static_cast<int>(window_mode));
        LOG_NOTICE(rootlogger, "capture_directory={}", capture_directory.toStdString());

        SystemStatusManager system_status_manager;

        CollageContext collage_context{scheduler, collage_directory.toStdString(), printer_settings.toStdString()};
        system_status_manager.registerClient(std::addressof(collage_context.systemStatusClient()));

        std::unique_ptr<RemoteTrigger> remote_trigger =
            std::make_unique<EspHomeRemoteTrigger>(std::make_unique<EspHomeClient>(trigger_button_host));
        std::unique_ptr<CameraLed> camera_led =
            std::make_unique<EspHomeCameraLed>(std::make_unique<EspHomeClient>(camera_led_host));
        std::shared_ptr<ICamera> camera;

        if (not developer_mode)
        {
            camera = std::make_shared<GPhoto2Camera>(scheduler);
        }
        else
        {
            camera = std::make_shared<MockCamera>();
        }

        CaptureManager capture_manager{
            *camera,
            collage_context,
        };
        system_status_manager.registerClient(std::addressof(camera->systemStatusClient()));

        LOG_NOTICE(rootlogger, "Start init capture controller");
        // auto capture_controller =
        //     std::make_shared<CaptureController>(scheduler,
        //                                         collage_directory.toStdString(),
        //                                         std::make_unique<ImageStorage>(capture_directory.toStdString()),
        //                                         camera,
        //                                         std::make_unique<CollagePrinter>(printer_settings.toStdString()));
        LOG_NOTICE(rootlogger, "End init capture controller");
        QQmlApplicationEngine engine;

        engine.setInitialProperties({
            {"visibility", window_mode},
        });
        auto &&app_state = engine.singletonInstance<ApplicationState *>("Photobox.Core", "ApplicationState");
        Q_ASSERT(app_state != nullptr);

        app_state->system_status_manager = std::addressof(system_status_manager);
        app_state->camera = camera;
        app_state->remote_trigger = remote_trigger.get();
        // app_state->capture_controller = capture_controller;
        app_state->camera_led = camera_led.get();
        app_state->capture_manager = std::addressof(capture_manager);

        engine.loadFromModule("Photobox.App", "MainNew");
        // engine.addImageProvider(QLatin1String("camera"), capture_controller->createImageProvider());

        app_return_code = app.exec();
    }
    return app_return_code;
}
