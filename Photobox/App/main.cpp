#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <ApplicationState.hpp>
#include <CameraImageProvider.hpp>
#include <ICamera.hpp>
#include <ImageStorage.hpp>
#include <MockCamera.hpp>

Q_IMPORT_QML_PLUGIN(Photobox_CorePlugin)

using namespace Pbox;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("PhotoBox"));
    QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge.photobox"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

    std::shared_ptr<ICamera> camera = std::make_shared<MockCamera>();

    ImageStorage image_storage{};
    QObject::connect(camera.get(), &ICamera::imageCaptured, &image_storage, &ImageStorage::onImageCaptured);

    QQmlApplicationEngine engine;

    auto &&app_state = engine.singletonInstance<ApplicationState *>("Photobox.Core", "ApplicationState");
    Q_ASSERT(app_state != nullptr);

    app_state->camera = camera;

    engine.loadFromModule("Photobox.App", "Main");
    engine.addImageProvider(QLatin1String("camera"), new CameraImageProvider(camera)); // qml engine takes ownership

    return app.exec();
}
