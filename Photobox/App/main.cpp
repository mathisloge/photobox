#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <ApplicationState.hpp>
#include <CameraImageProvider.hpp>
#include <CaptureController.hpp>
#include <CollagePrinter.hpp>
#include <GPhoto2Camera.hpp>
#include <ICamera.hpp>
#include <ImageStorage.hpp>
#include <MockCamera.hpp>
#include <PhotoTriggerClient.hpp>

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

    parser.process(app);

    const QString capture_directory = parser.value(capture_directory_option);
    const QString collage_directory = parser.value(collage_directory_option);
    const QString printer_settings = parser.value(printer_settings_option);

    qDebug() << "capture_directory =" << capture_directory;
    qDebug() << "collage_directory =" << collage_directory;
    qDebug() << "printer_settings =" << printer_settings;

    std::shared_ptr<PhotoTriggerClient> photo_trigger_client = std::make_shared<PhotoTriggerClient>();
    // std::shared_ptr<ICamera> camera = std::make_shared<GPhoto2Camera>();
    std::shared_ptr<ICamera> camera = std::make_shared<MockCamera>();

    auto capture_controller =
        std::make_shared<CaptureController>(collage_directory.toStdString(),
                                            std::make_unique<ImageStorage>(capture_directory.toStdString()),
                                            camera,
                                            std::make_unique<CollagePrinter>(printer_settings.toStdString()));

    QQmlApplicationEngine engine;

    auto &&app_state = engine.singletonInstance<ApplicationState *>("Photobox.Core", "ApplicationState");
    Q_ASSERT(app_state != nullptr);

    app_state->camera = camera;
    app_state->trigger_client = photo_trigger_client;
    app_state->capture_controller = capture_controller;

    engine.loadFromModule("Photobox.App", "Main");
    engine.addImageProvider(QLatin1String("camera"), capture_controller->createImageProvider());

    return app.exec();
}
