#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlExtensionPlugin>

Q_IMPORT_QML_PLUGIN(Photobox_LumixPlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("PhotoBox"));
    QCoreApplication::setOrganizationName(QStringLiteral("com.mathisloge.photobox"));
    QCoreApplication::setApplicationVersion(QStringLiteral(QT_VERSION_STR));

    QQmlApplicationEngine engine;
    engine.loadFromModule("Photobox.App", "Main");

    return app.exec();
}
