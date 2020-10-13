#include "mainwindow.h"
#include <QApplication>

#include <QDesktopServices>
#include <QHttpServer>

#include <QWebEngineSettings>
#include <QWebEngineProfile>


//NOTE: DISABLE Qt Quick Compiler in Project sttings if resources not loading in Release mode

static inline QString host(const QHttpServerRequest &request)
{
    return request.headers()[QStringLiteral("Host")].toString();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setApplicationName("ColorPie");
    QApplication::setOrganizationName("org.keshavnrj.ubuntu");
    QCoreApplication::setApplicationVersion("1");

    qputenv("QTWEBENGINE_CHROMIUM_FLAGS","--single-process");
#ifdef QT_DEBUG
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS","--remote-debugging-port=9421");
#endif
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS","--no-sandbox");

    static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
            && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
            && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }

    QHttpServer httpServer;

    httpServer.route("/<arg>", [] (const QUrl &url) {
        if(url.isEmpty()){
            return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg("v2.html"));
        }else{
            //qDebug()<<url;
            return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg(url.path()));
        }
    });

    httpServer.route("/html/<arg>", [] (const QUrl &url) {
        //qDebug()<<url;
        return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg(url.path()));
    });

    const auto port = httpServer.listen(QHostAddress::Any);
    if (!port) {
        qWarning() << QCoreApplication::translate(
                "QHttpServer", "Server failed to listen on a port.");
        return 0;
    }

    qWarning() << QCoreApplication::translate(
            "QHttpServer", "Running on http://127.0.0.1:%1/ (Press CTRL+C to quit)").arg(port);

    MainWindow w(nullptr,QString("http://127.0.0.1:%1").arg(port));
    w.show();

    return a.exec();
}
