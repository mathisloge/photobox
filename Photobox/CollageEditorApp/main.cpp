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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("PhotoBox Collage"));
    QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge-photobox.collage"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

    QQmlApplicationEngine engine;
    engine.loadFromModule("Photobox.CollageEditorApp", "Main");

    return app.exec();
}
