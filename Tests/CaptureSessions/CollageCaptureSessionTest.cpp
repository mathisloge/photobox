// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QSignalSpy>
#include <QTest>
#include <CollageCaptureSession.hpp>
#include <CollageFontCache.hpp>
#include <CollageRenderer.hpp>
#include <Pbox/Logger.hpp>
#include <catch2/catch_test_macros.hpp>
#include "../pixel_match.hpp"
#include "CollageContext.hpp"
#include "ImageStorage.hpp"
#include "Scheduler.hpp"
#include "TestAssets.hpp"

using namespace Pbox;

DEFINE_LOGGER(testlog);

TEST_CASE("Test CollageCaptureSession", "[UnitTest]")
{
    const auto asset_path = std::filesystem::path{kAssetsPath};
    CollageFontCache font_cache;
    init_lunasvg(font_cache);
    ImageStorage storage{std::filesystem::current_path()};
    Scheduler scheduler;
    CollageContext context{scheduler, storage, kAssetsPath};

    QTRY_VERIFY(context.systemStatusClient().systemStatus() == SystemStatusCode::Code::Ok);

    CollageCaptureSession session{context};
    QSignalSpy finished_spy{&session, &CollageCaptureSession::finished};

    session.imageCaptured(QImage{}, 1);
    session.imageSaved(asset_path / "capture_1.png");
    REQUIRE(finished_spy.size() == 0);
    session.imageCaptured(QImage{}, 2);
    session.imageSaved(asset_path / "capture_5.png");
    finished_spy.wait(std::chrono::seconds{context.settings().seconds_between_capture} + std::chrono::seconds{5});
    REQUIRE(finished_spy.size() == 1);

    // should not be added anywhere and the signal schould not be emitted
    // session.imageSaved(asset_path / "capture_5.png");
    // REQUIRE(finished_spy.size() == 1);

    const QImage expected_img =
        QImage{(asset_path / "expected_collage.png").c_str()}.convertToFormat(QImage::Format_RGBA8888);
    const QImage acutal_img =
        QImage{(std::filesystem::current_path() / "collage_session_test.png").c_str()}.convertToFormat(
            QImage::Format_RGBA8888);

    QImage diff_image;
    const auto diff = pixel_match(expected_img, acutal_img, PixelCompareOptions{}, diff_image);
    diff_image.save("collage_session_test_diff.png");
    REQUIRE(diff < 30);
}
