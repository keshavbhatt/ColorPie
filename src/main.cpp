#include "mainwindow.h"

#include <QApplication>
#include <QHttpServer>
#include <QHttpServerResponse>
#include <QTcpServer>

#include <memory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral(APPNAMESTR));
    QApplication::setOrganizationName(QStringLiteral("org.keshavnrj.ubuntu"));
    QApplication::setApplicationVersion(QStringLiteral(VERSIONSTR));

    // The color picker UI is a bundled web app served from the Qt resource
    // system through an embedded http server on a random localhost port.
    QHttpServer httpServer;
    // Registered before the catch-all: routes match in registration order.
    httpServer.route(QStringLiteral("/html/<arg>"), [](const QUrl &url) {
        return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg(url.path()));
    });
    httpServer.route(QStringLiteral("/<arg>"), [](const QUrl &url) {
        const QString file = url.isEmpty() ? QStringLiteral("v2.html") : url.path();
        return QHttpServerResponse::fromFile(QStringLiteral(":/html/%1").arg(file));
    });

    auto tcpServer = std::make_unique<QTcpServer>();
    if (!tcpServer->listen(QHostAddress::LocalHost) || !httpServer.bind(tcpServer.get())) {
        qWarning("%s: embedded http server failed to listen on a port.", APPNAMESTR);
        return 1;
    }
    const quint16 port = tcpServer->serverPort();
    tcpServer.release(); // owned by httpServer after a successful bind()

    qInfo("%s: serving ui on http://127.0.0.1:%d/", APPNAMESTR, port);

    MainWindow window(nullptr, QStringLiteral("http://127.0.0.1:%1").arg(port));
    window.show();

    return app.exec();
}
