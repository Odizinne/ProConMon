#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QIcon>

namespace Utils {

void getProControllerInfos(bool &proconConnected, int &batteryLevel, bool &charging);
QIcon getIcon(bool proconConnected, bool charging, int batteryLevel, QString &status);
}

#endif // UTILS_H
