#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSplitter>
#include <QUrl>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineView>

MainWindow::MainWindow(QWidget *parent, const QString &serverBase) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initColor = QColor("#00a");
    home = serverBase;
    this->setWindowIcon(QIcon(":/icons/icon-256.png"));
    this->setWindowTitle(qApp->applicationName()+" v"+qApp->applicationVersion());

    connect(ui->webView,&QWebEngineView::loadFinished,this,[=](const bool loaded){
        if(loaded && !app_init){
            loadColor(initColor.name());
            managerWidget->setFromHex6(initColor.name());
            app_init = true;
        }
    });

    QWebEngineProfile *weProfile = new QWebEngineProfile(QApplication::applicationName(),this);

    QWebEnginePage *webenginepage = new QWebEnginePage(weProfile, this);
    webenginepage->setBackgroundColor(QColor("#EFF0F1"));

    connect(webenginepage,&QWebEnginePage::titleChanged,this,[=](const QString &titleStr){
        this->setWindowTitle(QApplication::applicationName()+" | "+titleStr);
        QString hex6 = titleStr.split(">>").last().simplified().trimmed();
        QColor color("#"+hex6);
        if(color.isValid())
            managerWidget->setFromHex6("#"+hex6);
    });

    webenginepage->settings()->setAttribute(QWebEngineSettings::ShowScrollBars,false);

    //page should be set parent of profile to prevent
    //Release of profile requested but WebEnginePage still not deleted. Expect troubles !
    weProfile->setParent(webenginepage);

    ui->webView->setPage(webenginepage);

    ui->webView->setMinimumSize(500,500);

    managerWidget = new Manager(this);
    managerWidget->setMinimumSize(350,managerWidget->minimumSizeHint().height());

    managerWidget->initialize(initColor);
    connect(managerWidget,&Manager::colorChanged,this,[=](const QString &colorName){
        loadColor(colorName);
    });

    QSplitter *split1 = new QSplitter;
    split1->setObjectName("split1");
    split1->setOrientation(Qt::Horizontal);
    split1->setChildrenCollapsible(false);
    split1->addWidget(managerWidget);
    split1->addWidget(ui->webviewWidget);
    split1->setStretchFactor(0,2);
    split1->setStretchFactor(1,2);
    ui->centralWidget->layout()->addWidget(split1);
    if(settings.value("geometry").isValid()){
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("windowState").toByteArray());
    }
    ui->webviewWidget->layout()->setContentsMargins(9,0,0,0);
    ui->webView->load(QUrl(serverBase));
}

void MainWindow::loadColor(const QString &colorStr)
{
    QString js = "document.getElementById('color').value = '"+colorStr+"';"
                 "var event = new Event('keyup');"
                 "document.getElementById('color').dispatchEvent(event);"
                 "delete event;";
    ui->webView->page()->runJavaScript(js);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings.setValue("geometry",saveGeometry());
    settings.setValue("windowState", saveState());
    managerWidget->saveColors();
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
