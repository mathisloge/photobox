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
#include <QSvgRenderer>
#include <QSvgWidget>

Q_IMPORT_QML_PLUGIN(Photobox_CorePlugin)
Q_IMPORT_QML_PLUGIN(Photobox_UiPlugin)

using namespace Pbox;

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

    QSvgWidget x("/home/mathis/Downloads/TestImageEx.svg");

    for (int page = 0; page < numberOfPages; ++page)
    {

        painter.save();

        x.renderer()->render(&painter);

        painter.restore();

        if (page != lastPage)
            printer.newPage();
    }

    painter.end();

    printer.setOutputFormat(QPrinter::OutputFormat::PdfFormat);
}
} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("PhotoBox Collage"));
    QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge-photobox.collage"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

    testPrint();

    return app.exec();
}
