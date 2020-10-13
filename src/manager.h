#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QSettings>
#include <QMessageBox>
#include <QLayoutItem>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

#include "QtColorWidgets/color_line_edit.hpp"
#include "QtColorWidgets/ColorPreview"
#include "QtColorWidgets/ColorDialog"
#include "QtColorWidgets/ColorListWidget"

#include "utils.h"
#include "supportedinputs.h"

namespace Ui {
class Manager;
}

class Manager : public QWidget
{
    Q_OBJECT

signals:
    void colorChanged(QString colorName);

public:
    explicit Manager(QWidget *parent = nullptr);
    ~Manager();

public slots:
    void initialize(QColor color);
    void setFromHex6(QString hex6);
    void saveColors();

private slots:
    void on_colorControl_textChanged(const QString &arg1);

    void on_supportedInputs_clicked();

    void setLayoutVisible(QLayoutItem *item, bool visible);
    void loadColors();
private:
    Ui::Manager *ui;


    color_widgets::ColorLineEdit *colorLineEdit = nullptr;
    color_widgets::ColorDialog * colorDialog = nullptr;
    color_widgets::ColorListWidget * colorListWidget = nullptr;

    QList<QColor> colorCollection;
    QSettings settings;

};

#endif // MANAGER_H
