#include "utils.h"

#include <QDir>
#include <QFile>
#include <QStandardPaths>

namespace utils {

QString returnPath(const QString &pathname)
{
    const QString dataPath =
        QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    const QString path = dataPath + QDir::separator() + pathname;
    QDir().mkpath(path);
    return path + QDir::separator();
}

void saveJson(const QJsonDocument &document, const QString &fileName)
{
    QFile jsonFile(fileName);
    if (jsonFile.open(QFile::WriteOnly))
        jsonFile.write(document.toJson());
}

QJsonDocument loadJson(const QString &fileName)
{
    QFile jsonFile(fileName);
    if (jsonFile.open(QFile::ReadOnly))
        return QJsonDocument::fromJson(jsonFile.readAll());
    return QJsonDocument();
}

} // namespace utils
