// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCoreApplication>
#include <Pbox/SetupLogging.hpp>
#include <catch2/catch_session.hpp>

int main(int argc, char **argv)
{
    Pbox::setupLogging();
    QCoreApplication app(argc, argv);
    return Catch::Session().run(argc, argv);
}
