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

namespace
{
void testPrint();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("PhotoBox"));
    QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge.photobox"));
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

    engine.loadFromModule("Photobox.App", "Main");
    engine.addImageProvider(QLatin1String("camera"), new CameraImageProvider(camera)); // qml engine takes ownership

    testPrint();

    return app.exec();
}

#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
namespace
{
void testPrint()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName("print.pdf");

    QPainter painter;
    painter.begin(&printer);

    int numberOfPages = 2;
    int lastPage = numberOfPages - 1;

    for (int page = 0; page < numberOfPages; ++page)
    {

        painter.save();
        painter.drawRect(10, 10, 100, 100);
        painter.setPen(Qt::blue);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(100, 100, "Qt");

        painter.restore();

        if (page != lastPage)
            printer.newPage();
    }

    painter.end();

    printer.setOutputFormat(QPrinter::OutputFormat::PdfFormat);
}
} // namespace
