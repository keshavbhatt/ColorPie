#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, const QString &serverBase = QString());
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void setStyle(const QString &fname);
    void loadColor(const QString &colorStr);

private:
    Ui::MainWindow *ui;
    QString home;
    QSettings settings;
    bool app_init = false;
    Manager *managerWidget = nullptr;
    QColor initColor;
};

#endif // MAINWINDOW_H
