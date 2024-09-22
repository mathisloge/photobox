#include "CollagePrinter.hpp"
#include <QPrinter>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
void from_json(const json &j, QMarginsF &l)
{
    l.setTop(j.at("top").get<qreal>());
    l.setTop(j.at("bottom").get<qreal>());
    l.setTop(j.at("left").get<qreal>());
    l.setTop(j.at("right").get<qreal>());
}

void from_json(const json &j, QPageLayout &l)
{
    l.setPageSize(j.at("pageSize").get<QPageSize::PageSizeId>());
    l.setOrientation(j.at("orientation").get<QPageLayout::Orientation>());
    l.setUnits(j.at("units").get<QPageLayout::Unit>());
    l.setMargins(j.at("margins").get<QMarginsF>());
}

void from_json(const json &j, QPrinter &l)
{
    l.setPrinterName(QString::fromStdString(j.at("name").get<std::string>()));
    QPageLayout layout;
    j.at("pageLayout").get_to(layout);
    l.setPageLayout(layout);
    l.setFullPage(j.at("fullPage").get<bool>());
}

namespace Pbox
{
CollagePrinter::CollagePrinter(const std::filesystem::path &settings)
    : printer_{std::make_unique<QPrinter>()}
{
    std::ifstream settings_file{settings};
    nlohmann::json json;
    settings_file >> json;

    json.get_to(*printer_);
}
} // namespace Pbox