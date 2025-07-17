// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CollagePrinter.hpp"
#include <QPainter>
#include <QPrinter>
#include <fstream>
#include <nlohmann/json.hpp>
#include "CollageRenderer.hpp"

using json = nlohmann::json;

static void from_json(const json &j, QMarginsF &l)
{
    l.setTop(j.at("top").get<qreal>());
    l.setTop(j.at("bottom").get<qreal>());
    l.setTop(j.at("left").get<qreal>());
    l.setTop(j.at("right").get<qreal>());
}

static void from_json(const json &j, QPageSize &p)
{
    p = QPageSize{j.at("pageSizeId").get<QPageSize::PageSizeId>()};
}

static void from_json(const json &j, QPageLayout &l)
{
    l.setPageSize(j.at("pageSize").get<QPageSize>());
    l.setOrientation(j.at("orientation").get<QPageLayout::Orientation>());
    l.setUnits(j.at("units").get<QPageLayout::Unit>());
    l.setMargins(j.at("margins").get<QMarginsF>());
}

static void from_json(const json &j, QPrinter &l)
{
    const auto name = j.at("name").get<std::string>();
    if (name.empty())
    {
        l.setOutputFileName("collage.pdf");
    }
    else
    {
        l.setPrinterName(QString::fromStdString(name));
    }
    QPageLayout layout;
    j.at("pageLayout").get_to(layout);
    l.setPageLayout(layout);
    l.setFullPage(j.at("fullPage").get<bool>());
    l.setResolution(j.at("resolution").get<int>());
}

namespace Pbox
{
CollagePrinter::CollagePrinter(const std::filesystem::path &settings)
    : printer_{std::make_unique<QPrinter>(QPrinter::HighResolution)}
{
    std::ifstream settings_file{settings};
    nlohmann::json json;
    settings_file >> json;

    json.get_to(*printer_);
    printer_->setResolution(1000);
}

CollagePrinter::~CollagePrinter() = default;

void CollagePrinter::print(CollageRenderer &renderer)
{
    QPainter painter;
    painter.begin(printer_.get());
    renderer.render(&painter, static_cast<float>(printer_->width()), static_cast<float>(printer_->height()));
    painter.end();
}
} // namespace Pbox
