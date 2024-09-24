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

    std::shared_ptr<PhotoTriggerClient> photo_trigger_client = std::make_shared<PhotoTriggerClient>();
    std::shared_ptr<ICamera> camera = std::make_shared<GPhoto2Camera>();
    // std::shared_ptr<ICamera> camera = std::make_shared<MockCamera>();

    auto capture_controller = std::make_shared<CaptureController>(
        "/home/mathis/dev/photobox2/build/Photobox/CollageEditorApp/collage",
        std::make_unique<ImageStorage>(std::filesystem::current_path()),
        camera,
        std::make_unique<CollagePrinter>(
            "/home/mathis/dev/photobox2/build/Photobox/CollageEditorApp/printer_settings.json"));

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
