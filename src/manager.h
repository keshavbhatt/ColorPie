#ifndef MANAGER_H
#define MANAGER_H

#include <QWidget>
#include <QClipboard>
#include <QClipboard>
#include <QMessageBox>

#include "QtColorWidgets/ColorWheel"
#include "QtColorWidgets/color_line_edit.hpp"
#include "QtColorWidgets/ColorPreview"

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
private slots:
    void on_colorControl_textChanged(const QString &arg1);

    void on_supportedInputs_clicked();

private:
    Ui::Manager *ui;
    QClipboard *clipboard = nullptr;

    color_widgets::ColorWheel *colorWheel = nullptr;
    color_widgets::ColorLineEdit *colorLineEdit = nullptr;
    color_widgets::ColorPreview *colorPreview = nullptr;

};

#endif // MANAGER_H
