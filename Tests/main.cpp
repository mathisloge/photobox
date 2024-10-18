#include <QtGui/QGuiApplication>
#include <catch2/catch_session.hpp>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    return Catch::Session().run(argc, argv);
}