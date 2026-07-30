#ifndef SCREENPICKER_H
#define SCREENPICKER_H

#include <QColor>
#include <QObject>
#include <QtDBus/QDBusPendingCallWatcher>

// Picks a color from anywhere on the screen through the XDG desktop portal
// (org.freedesktop.portal.Screenshot.PickColor). Unlike grabbing the mouse
// and reading the framebuffer, this works on Wayland as well as X11.
class ScreenPicker : public QObject
{
    Q_OBJECT

public:
    explicit ScreenPicker(QObject *parent = nullptr);

public slots:
    void pickColor();

signals:
    void colorPicked(const QColor &color);
    void errorOccurred(const QString &message);

private slots:
    void onPortalRequestFinished(QDBusPendingCallWatcher *watcher);
    void onPortalResponse(uint response, const QVariantMap &results);

private:
    QString m_requestPath;
};

#endif // SCREENPICKER_H
