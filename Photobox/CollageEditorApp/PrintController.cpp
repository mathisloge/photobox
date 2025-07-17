// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PrintController.hpp"
#include <QPageLayout>
#include <QPageSetupDialog>
#include <QPrinter>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
void to_json(json &j, const QString &str)
{
    j = str.toStdString();
}

void to_json(json &j, const QMarginsF &p)
{
    j = json{{"top", p.top()}, {"bottom", p.bottom()}, {"left", p.left()}, {"right", p.right()}};
}

void to_json(json &j, const QPageSize &p)
{
    j = json{{"pageSizeId", p.id()}};
}

void to_json(json &j, const QPageLayout &l)
{
    j = json{
        {"pageSize", l.pageSize()}, {"orientation", l.orientation()}, {"units", l.units()}, {"margins", l.margins()}};
}

void to_json(json &j, const QPrinter &l)
{
    j = json{{"name", l.printerName()},
             {"pageLayout", l.pageLayout()},
             {"fullPage", l.fullPage()},
             {"resolution", l.resolution()}};
}

namespace Pbox
{

void PrintController::showPrintSettings()
{
    /// printer.setOutputFileName("print.pdf");
    QPageSetupDialog print_dialog;
    if (print_dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    // printer.setFullPage(bool)
    auto &&printer = *print_dialog.printer();

    json printer_settings = printer;

    std::ofstream o("printer_settings.json");
    o << std::setw(4) << printer_settings << std::endl;
} // namespace Pbox

} // namespace Pbox
