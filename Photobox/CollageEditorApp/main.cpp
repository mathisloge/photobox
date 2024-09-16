#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <ApplicationState.hpp>
#include <CameraImageProvider.hpp>
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
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("PhotoBox"));
    QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge-photobox.collage"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

    std::shared_ptr<PhotoTriggerClient> photo_trigger_client = std::make_shared<PhotoTriggerClient>();
    // std::shared_ptr<ICamera> camera = std::make_shared<GPhoto2Camera>();
    std::shared_ptr<ICamera> camera = std::make_shared<MockCamera>();

    ImageStorage image_storage{std::filesystem::current_path()};
    QObject::connect(camera.get(), &ICamera::imageCaptured, &image_storage, &ImageStorage::onImageCaptured);

    QQmlApplicationEngine engine;

    auto &&app_state = engine.singletonInstance<ApplicationState *>("Photobox.Core", "ApplicationState");
    Q_ASSERT(app_state != nullptr);

    app_state->camera = camera;
    app_state->trigger_client = photo_trigger_client;

    engine.loadFromModule("Photobox.CollageEditorApp", "Main");

    return app.exec();
}
