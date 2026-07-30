#ifndef UTILS_H
#define UTILS_H

#include <QJsonDocument>
#include <QString>

namespace utils {

// Returns the writable app-data subdirectory named pathname (created on
// demand), with a trailing separator.
QString returnPath(const QString &pathname);

void saveJson(const QJsonDocument &document, const QString &fileName);
QJsonDocument loadJson(const QString &fileName);

} // namespace utils

#endif // UTILS_H
