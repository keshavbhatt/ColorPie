#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QUrl>
#include <QSplitter>
#include <requestinterceptor.h>

MainWindow::MainWindow(QWidget *parent,QString serverBase) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initColor = QColor("#00a");
    home = serverBase;
    this->setWindowIcon(QIcon(":/icons/icon-256.png"));
    this->setWindowTitle(qApp->applicationName()+" v"+qApp->applicationVersion());

//    ui->webView->setStyleSheet("background-color:#181818;");

    connect(ui->webView,&QWebEngineView::urlChanged,[=](const QUrl url){
        qDebug()<<url.toString();
    });

    connect(ui->webView,&QWebEngineView::loadFinished,[=](const bool loaded){
        if(loaded && !app_init){
            loadColor(initColor.name());
            managerWidget->setFromHex6(initColor.name());
            app_init = true;
        }
    });

    QWebEngineProfile *weProfile = new QWebEngineProfile(QApplication::applicationName(),this);

    weProfile->setHttpUserAgent("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:72.0) Gecko/20100101 Firefox/72.0");
    weProfile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);

    WebEnginePage *webenginepage = new WebEnginePage(weProfile, this);
//    webenginepage->setBackgroundColor(QColor("#181818"));

    connect(webenginepage,&WebEnginePage::titleChanged,[=](const QString titleStr){
        this->setWindowTitle(QApplication::applicationName()+" | "+QString(titleStr));
        QString hex6 = QString(titleStr).split(">>").last().simplified().trimmed();
        QColor color("#"+hex6);
        if(color.isValid())
            managerWidget->setFromHex6("#"+hex6);
    });

//    RequestInterceptor *interceptor = new RequestInterceptor(weProfile);
//    weProfile->setUrlRequestInterceptor(interceptor);

    webenginepage->settings()->setAttribute(QWebEngineSettings::ShowScrollBars,false);

    //page should be set parent of profile to prevent
    //Release of profile requested but WebEnginePage still not deleted. Expect troubles !
    weProfile->setParent(webenginepage);

    ui->webView->setPage(webenginepage);
    setStyle(":/light.qss");

    ui->webView->setMinimumSize(500,500);

    managerWidget = new Manager(this);
    managerWidget->setMinimumSize(350,managerWidget->minimumSizeHint().height());
    managerWidget->setStyleSheet("QWidget{background-image:url(:/html/texture.png)}");

    managerWidget->initialize(initColor);
    connect(managerWidget,&Manager::colorChanged,[=](QString colorName){
        loadColor(colorName);
    });

    QSplitter *split1 = new QSplitter;
    split1->setObjectName("split1");
    split1->setOrientation(Qt::Horizontal);
    split1->addWidget(managerWidget);
    split1->addWidget(ui->webView);
    split1->setStretchFactor(0,2);
    split1->setStretchFactor(1,2);
    ui->centralWidget->layout()->addWidget(split1);


    if(settings.value("geometry").isValid()){
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("windowState").toByteArray());
    }

    ui->webView->load(serverBase+"/v2.html");
}
void MainWindow::loadColor(QString colorStr)
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
    QMainWindow::closeEvent(event);
}

void MainWindow::setStyle(QString fname)
{
    QFile styleSheet(fname);
    if (!styleSheet.open(QIODevice::ReadOnly)) {
        qWarning("Unable to open file");
    return; }
    qApp->setStyleSheet(styleSheet.readAll());
    styleSheet.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

