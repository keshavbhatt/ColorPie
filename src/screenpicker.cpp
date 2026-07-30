#include "screenpicker.h"

#include <QtDBus/QDBusArgument>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusPendingReply>

// The portal returns the picked color as a DBus (ddd) struct.
struct ColorStruct {
    double r;
    double g;
    double b;
};
Q_DECLARE_METATYPE(ColorStruct)

QDBusArgument &operator<<(QDBusArgument &argument, const ColorStruct &color)
{
    argument.beginStructure();
    argument << color.r << color.g << color.b;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ColorStruct &color)
{
    argument.beginStructure();
    argument >> color.r >> color.g >> color.b;
    argument.endStructure();
    return argument;
}

ScreenPicker::ScreenPicker(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<ColorStruct>("ColorStruct");
    qDBusRegisterMetaType<ColorStruct>();
}

void ScreenPicker::pickColor()
{
    QDBusMessage message = QDBusMessage::createMethodCall(
        "org.freedesktop.portal.Desktop",
        "/org/freedesktop/portal/desktop",
        "org.freedesktop.portal.Screenshot",
        "PickColor");

    message << ""; // parent window handle
    message << QVariantMap(); // options

    QDBusPendingCall call = QDBusConnection::sessionBus().asyncCall(message);
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);

    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, &ScreenPicker::onPortalRequestFinished);
}

void ScreenPicker::onPortalRequestFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<QDBusObjectPath> reply = *watcher;
    if (reply.isError()) {
        emit errorOccurred(QString("Portal error: %1").arg(reply.error().message()));
    } else {
        m_requestPath = reply.value().path();

        QDBusConnection::sessionBus().connect(
            "org.freedesktop.portal.Desktop",
            m_requestPath,
            "org.freedesktop.portal.Request",
            "Response",
            this,
            SLOT(onPortalResponse(uint, QVariantMap)));
    }
    watcher->deleteLater();
}

void ScreenPicker::onPortalResponse(uint response, const QVariantMap &results)
{
    if (response == 0 && results.contains("color")) {
        QDBusArgument arg = results["color"].value<QDBusArgument>();
        ColorStruct color;
        arg >> color;

        QColor qtColor;
        qtColor.setRgbF(color.r, color.g, color.b);
        emit colorPicked(qtColor);
    }

    QDBusConnection::sessionBus().disconnect(
        "org.freedesktop.portal.Desktop",
        m_requestPath,
        "org.freedesktop.portal.Request",
        "Response",
        this,
        SLOT(onPortalResponse(uint, QVariantMap)));
}
