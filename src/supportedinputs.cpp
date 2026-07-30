#include "supportedinputs.h"
#include "ui_supportedinputs.h"

SupportedInputs::SupportedInputs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SupportedInputs)
{
    ui->setupUi(this);

    // No hardcoded cell colors: the content must stay readable with any
    // system theme (light or dark).
    static const struct {
        const char *system;
        const char *format;
    } formats[] = {
        {"cmyk", "cmyk 39 0 39 7"},
        {"grayscale", "78"},
        {"hex3", "#9E9"},
        {"hex4", "#9E9F"},
        {"hex6", "#90EE90"},
        {"hex8", "#90EE90FF"},
        {"html", "Light Green"},
        {"hsl", "hsl 120 73.4 74.9"},
        {"hsv", "hsv 120 39.4 93.3"},
        {"lab", "lab 86.5 -46.3 36.9"},
        {"pantone", "pantone 358C"},
        {"ral", "ral 6019"},
        {"rgb", "rgb 144 238 144"},
        {"rgba", "rgba 144 238 144 1"},
        {"rgb decimal", "rgb decimal 6812065"},
        {"w", "w 544"},
        {"xyz", "xyz 44 69 45"},
        {"yuv", "yuv 180 113.2 73.5"},
    };

    QString rows;
    for (const auto &entry : formats) {
        rows += QStringLiteral(
                    "<tr><td align=\"center\">%1</td>"
                    "<td align=\"center\">%2</td></tr>")
                    .arg(QLatin1String(entry.system), QLatin1String(entry.format));
    }

    ui->textBrowser->setHtml(QStringLiteral(
        "<table width=\"100%\" cellpadding=\"6\">"
        "<tr><th>Color system</th><th>String Format</th></tr>%1</table>")
        .arg(rows));
}

SupportedInputs::~SupportedInputs()
{
    delete ui;
}
