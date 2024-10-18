#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <QWindow>
#include <ApplicationState.hpp>
#include <CameraImageProvider.hpp>
#include <CaptureController.hpp>
#include <CollagePrinter.hpp>
#include <EspHomeClient.hpp>
#include <EspHomeRemoteTrigger.hpp>
#include <GPhoto2Camera.hpp>
#include <ICamera.hpp>
#include <ImageStorage.hpp>
#include <MockCamera.hpp>

Q_IMPORT_QML_PLUGIN(Photobox_CorePlugin)
Q_IMPORT_QML_PLUGIN(Photobox_UiPlugin)

using namespace Pbox;

int main(int argc, char *argv[])
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

    QCommandLineOption printer_settings_option(
        QStringList() << "printer-settings", "The printer_settings.json file", "path to file", "printer_settings.json");
    parser.addOption(printer_settings_option);

    QCommandLineOption developer_option(QStringList{"dev"}, "Use developer mode");
    parser.addOption(developer_option);

    QCommandLineOption trigger_button_host_option(
        QStringList() << "trigger-btn", "The hostname of the trigger button", "url", "http://192.168.0.31");
    parser.addOption(trigger_button_host_option);

    QCommandLineOption fullscreen_option{QStringList{QStringLiteral("f"), QStringLiteral("fullscreen")},
                                         QStringLiteral("shows the application in fullscreen mode")};
    parser.addOption(fullscreen_option);

    parser.process(app);

    const QString capture_directory = parser.value(capture_directory_option);
    const QString collage_directory = parser.value(collage_directory_option);
    const QString printer_settings = parser.value(printer_settings_option);
    const bool developer_mode = parser.isSet(developer_option);
    const QString trigger_button_host = parser.value(trigger_button_host_option);
    const QWindow::Visibility window_mode =
        parser.isSet(fullscreen_option) ? QWindow::Visibility::FullScreen : QWindow::Visibility::Windowed;

    qDebug() << "capture_directory =" << capture_directory;
    qDebug() << "collage_directory =" << collage_directory;
    qDebug() << "printer_settings =" << printer_settings;
    qDebug() << "developer_mode=" << developer_mode;
    qDebug() << "trigger_button_host=" << trigger_button_host;
    qDebug() << "window_mode=" << window_mode;

    std::unique_ptr<RemoteTrigger> remote_trigger =
        std::make_unique<EspHomeRemoteTrigger>(std::make_unique<EspHomeClient>(trigger_button_host));
    std::shared_ptr<ICamera> camera;
    if (not developer_mode)
    {
        camera = std::make_shared<GPhoto2Camera>();
    }
    else
    {
        camera = std::make_shared<MockCamera>();
    }

    auto capture_controller =
        std::make_shared<CaptureController>(collage_directory.toStdString(),
                                            std::make_unique<ImageStorage>(capture_directory.toStdString()),
                                            camera,
                                            std::make_unique<CollagePrinter>(printer_settings.toStdString()));

    QQmlApplicationEngine engine;

    engine.setInitialProperties({
        {"visibility", window_mode},
    });
    auto &&app_state = engine.singletonInstance<ApplicationState *>("Photobox.Core", "ApplicationState");
    Q_ASSERT(app_state != nullptr);

    app_state->camera = camera;
    app_state->remote_trigger = remote_trigger.get();
    app_state->capture_controller = capture_controller;

    engine.loadFromModule("Photobox.App", "Main");
    engine.addImageProvider(QLatin1String("camera"), capture_controller->createImageProvider());

    return app.exec();
}
