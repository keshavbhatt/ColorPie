#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineScript>
#include <QWebEngineProfile>
#include <QWebEngineView>

#include <QFile>
#include <QSettings>

#include "manager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,QString serverBase="");
    ~MainWindow();

protected slots:
    void closeEvent(QCloseEvent *event);
private slots:
    void setStyle(QString fname);
    void loadColor(QString colorStr);
private:
    Ui::MainWindow *ui;
    QString home;
    QSettings settings;
    bool app_init = false;
    Manager * managerWidget = nullptr;
    QColor initColor;

};

#endif // MAINWINDOW_H
