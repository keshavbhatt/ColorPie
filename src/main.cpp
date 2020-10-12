#include "mainwindow.h"
#include <QApplication>

#include <QDesktopServices>
#include <QHttpServer>

#include <QWebEngineSettings>
#include <QWebEngineProfile>




static inline QString host(const QHttpServerRequest &request)
{
    return request.headers()[QStringLiteral("Host")].toString();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setApplicationName("ColorConv");
    QApplication::setOrganizationName("org.keshavnrj.ubuntu");
    QCoreApplication::setApplicationVersion("1");

    static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
            && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }


    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);

    QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
    QWebEngineProfile::defaultProfile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);



    QHttpServer httpServer;

    httpServer.route("/<arg>", [] (const QUrl &url) {
        if(url.isEmpty()){
            return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg("index.html"));
        }else{
            qDebug()<<url;
            return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg(url.path()));
        }
    });

    httpServer.route("/html/<arg>", [] (const QUrl &url) {
        qDebug()<<url;
        return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg(url.path()));
    });

    const auto port = httpServer.listen(QHostAddress::Any);
    if (!port) {
        qWarning() << QCoreApplication::translate(
                "QHttpServerExample", "Server failed to listen on a port.");
        return 0;
    }

    qWarning() << QCoreApplication::translate(
            "QHttpServerExample", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);

    MainWindow w(nullptr,QString("http://127.0.0.1:%1").arg(port));
    w.show();

    return a.exec();
}
