#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>
#include <ApplicationState.hpp>
#include <CameraImageProvider.hpp>
#include <GPhoto2Camera.hpp>
#include <ICamera.hpp>
#include <ImageStorage.hpp>
#include <MockCamera.hpp>
#include <PhotoTriggerClient.hpp>

#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include "CollageRenderer.hpp"

Q_IMPORT_QML_PLUGIN(Photobox_CorePlugin)
Q_IMPORT_QML_PLUGIN(Photobox_UiPlugin)

using namespace Pbox;

namespace
{
void testPrint()
{
    QPrinter printer(QPrinter::PrinterMode::HighResolution);
    /// printer.setOutputFileName("print.pdf");
    QPrintDialog printDialog(&printer);
    if (printDialog.exec() != QDialog::Accepted)
    {
        return;
    }
    // todo save printer configuration to json

    QPainter painter;
    painter.begin(&printer);

    CollageRenderer x;
    x.loadDocument("/home/mathis/Downloads/test.svg");

    painter.save();

    x.render(&painter, printer.width(), printer.height());

    painter.restore();

    painter.end();
}
} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("PhotoBox Collage"));
    QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge-photobox.collage"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

    // QTimer::singleShot(0, []() { testPrint(); });

    QQmlApplicationEngine engine;
    engine.loadFromModule("Photobox.CollageEditorApp", "Main");

    return app.exec();
}