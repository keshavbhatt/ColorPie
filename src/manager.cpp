#include "manager.h"
#include "ui_manager.h"

#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QClipboard>
#include <QJsonValue>
#include <QPushButton>
#include <QShortcut>

#include "supportedinputs.h"
#include "utils.h"

using namespace color_widgets;

Manager::Manager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manager)
{
    ui->setupUi(this);

    colorDialog  = new ColorDialog(this);
    colorListWidget  = new ColorListWidget(this);

    connect(colorListWidget,&ColorListWidget::colorsChanged,[=](const QList<QColor>&colorList){
        colorCollection = colorList;
    });

    // Clicking a saved swatch loads it as the current color.
    connect(colorListWidget, &ColorListWidget::colorClicked, colorDialog,
            [=](const QColor &color) { colorDialog->setColor(color); });

    colorDialog->layout()->setContentsMargins(0,0,0,0);
    colorDialog->setButtonMode(ColorDialog::NoButton);
    colorDialog->setPreviewDisplayMode(ColorPreview::AllAlpha);

    // The saved-color swatches and the web view already preview the current
    // color; the strip below the wheel is redundant.
    if (auto *preview = colorDialog->findChild<ColorPreview *>("preview"))
        preview->hide();

    // Move the dialog's "Pick" button up into the input row, before the
    // "Supported inputs" button.
    if (auto *pickButton = colorDialog->findChild<QPushButton *>("picker")) {
        pickButton->setToolTip(tr("Pick a color from anywhere on the screen (Ctrl+P)"));
        ui->inputLayout->insertWidget(1, pickButton);
    }
    ui->supportedInputs->setToolTip(tr("List of color formats the input field understands"));

    //gridLayout modifications
    QGridLayout *gridLayout = colorDialog->findChild<QGridLayout*>("gridLayout");
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    for (int i = 0; i < gridLayout->count(); ++i) {
        setLayoutVisible(gridLayout->itemAt(i),false);
    }

    connect(colorDialog,&ColorDialog::colorChanged,[=](const QColor color){
        Q_EMIT colorChanged(color.name());
        colorListWidget->setColor(color);
        syncColorControl(color.name());
    });

    // Screen color picking goes through the XDG desktop portal so it works
    // on Wayland as well as X11.
    screenPicker = new ScreenPicker(this);
    connect(colorDialog, &ColorDialog::screenColorPickRequested,
            screenPicker, &ScreenPicker::pickColor);
    connect(screenPicker, &ScreenPicker::colorPicked, colorDialog,
            [=](const QColor &color) { colorDialog->setColor(color); });
    connect(screenPicker, &ScreenPicker::errorOccurred, this,
            [](const QString &message) { qWarning() << message; });

    ui->colorBoxLayout->addWidget(colorDialog);
    ui->savedColorsBoxLayout->addWidget(colorListWidget);

    // Hint when a hex-style input can't be parsed (other formats are
    // resolved by the converter and can't be validated here).
    connect(ui->colorControl, &QLineEdit::editingFinished, this, [this]{
        const QString text = ui->colorControl->text().simplified();
        const bool badHex = text.startsWith('#') && !QColor(text).isValid();
        ui->colorControl->setStyleSheet(
            badHex ? QStringLiteral("QLineEdit { border: 1px solid #d9534f; }")
                   : QString());
    });
    connect(ui->colorControl, &QLineEdit::textEdited, this,
            [this]{ ui->colorControl->setStyleSheet(QString()); });

    // Keyboard shortcuts (window-wide)
    auto *pickShortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+P")), this);
    connect(pickShortcut, &QShortcut::activated, screenPicker, &ScreenPicker::pickColor);

    auto *saveShortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+D")), this);
    connect(saveShortcut, &QShortcut::activated,
            colorListWidget, &ColorListWidget::append);

    auto *copyShortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+Shift+C")), this);
    connect(copyShortcut, &QShortcut::activated, this, [this]{
        QApplication::clipboard()->setText(colorDialog->color().name());
    });

    auto *focusShortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+L")), this);
    connect(focusShortcut, &QShortcut::activated, this, [this]{
        ui->colorControl->setFocus();
        ui->colorControl->selectAll();
    });

    if (settings.value("managerSplitterState").isValid())
        ui->managerSplitter->restoreState(
            settings.value("managerSplitterState").toByteArray());
    // Persist on every drag too, so adjustments survive unclean exits.
    connect(ui->managerSplitter, &QSplitter::splitterMoved,
            this, &Manager::saveSettings);

    loadColors();
}

//helper
void Manager::setLayoutVisible(QLayoutItem *item, bool visible)
{
    if (auto widget = item->widget())
        return widget->setVisible(visible);
    if (auto layout = item->layout())
        for (int i = 0; i < layout->count(); ++i)
        setLayoutVisible(layout->itemAt(i), visible);
}

void Manager::setFromHex6(QString hex6)
{
    colorDialog->blockSignals(true);
    colorDialog->setColor(QColor(hex6));
    colorListWidget->setColor(QColor(hex6));
    colorDialog->blockSignals(false);
    syncColorControl(QColor(hex6).name());
}

// Keep the input field showing the current color without disturbing the
// user while they are typing in it (setText resets isModified).
void Manager::syncColorControl(const QString &text)
{
    if (ui->colorControl->hasFocus() && ui->colorControl->isModified())
        return;
    QSignalBlocker blocker(ui->colorControl);
    ui->colorControl->setText(text);
}

void Manager::initialize(QColor color)
{
    ui->colorControl->setText(color.name());
}

Manager::~Manager()
{
    delete ui;
}

void Manager::on_colorControl_textChanged(const QString &arg1)
{
    Q_EMIT colorChanged(arg1);
    if(arg1.length()==7){
        QColor color(arg1);
        if(color.isValid()){
                colorDialog->setColor(color);
        }
    }
}

void Manager::on_supportedInputs_clicked()
{
    // Reuse the open dialog instead of stacking a new copy per click.
    if (!supportedInputsDialog) {
        supportedInputsDialog = new SupportedInputs(this);
        supportedInputsDialog->setWindowTitle(
            QApplication::applicationName()+" | "+"Supported Input");
        supportedInputsDialog->setAttribute(Qt::WA_DeleteOnClose);
    }
    supportedInputsDialog->show();
    supportedInputsDialog->raise();
    supportedInputsDialog->activateWindow();
}

void Manager::saveSettings()
{
    settings.setValue("managerSplitterState", ui->managerSplitter->saveState());
}

void Manager::saveColors()
{
    QJsonArray dataArray;
    foreach (auto color, colorCollection) {
        dataArray.append(QJsonValue(color.name()));
    }
    QJsonDocument doc(dataArray);
    QString filepath = utils::returnPath("savedColors")+"colors.json";
    utils::saveJson(doc,filepath);
}

void Manager::loadColors()
{
    QString filepath = utils::returnPath("savedColors")+"colors.json";
    QJsonDocument doc = utils::loadJson(filepath);
    if(doc.isNull()==false){
        QJsonArray dataArray = doc.array();
        foreach (auto color, dataArray) {
            colorCollection.append(QColor(color.toString()));
        }
    }
    colorListWidget->setColors(colorCollection);
}
