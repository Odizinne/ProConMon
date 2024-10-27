#include <QCoreApplication>
#include <QDebug>
#include <hidapi.h>
#include <thread>
#include <chrono>
#include "utils.h"
#include <QSettings>

QString getTheme()
{
    QSettings settings(
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        QSettings::NativeFormat);
    int value = settings.value("AppsUseLightTheme", 1).toInt();

    return (value == 0) ? "light" : "dark";
}

void Utils::getProControllerInfos(bool &proconConnected, int &batteryLevel, bool &charging) {
    const int VID = 0x057E;
    const int PID = 0x2009;

    // Initialize HIDAPI
    if (hid_init() != 0) {
        qDebug() << "Failed to initialize HIDAPI.";
        proconConnected = false;
        batteryLevel = -1;
        charging = false;
        return;
    }

    // Open the device
    hid_device* controller = hid_open(VID, PID, nullptr);
    if (!controller) {
        qDebug() << "Failed to open device. Make sure the controller is connected.";
        proconConnected = false;
        batteryLevel = -1;
        charging = false;
        return;
    }
    proconConnected = true;

    // Set non-blocking mode
    hid_set_nonblocking(controller, 1);

    // Initialize communication
    unsigned char init_packet[2] = {0x80, 0x01};
    hid_write(controller, init_packet, sizeof(init_packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    unsigned char request_packet[3] = {0x01, 0x40, 0x03};
    hid_write(controller, request_packet, sizeof(request_packet));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Read data
    unsigned char data[64];
    int res = hid_read(controller, data, sizeof(data));
    if (res > 0) {
        unsigned char battery_info = data[2];
        batteryLevel = (battery_info & 0xF0) >> 4;
        qDebug() << batteryLevel;
        switch (batteryLevel) {
        case 9:
            batteryLevel = 100;
            charging = true;
            break;
        case 8:
            batteryLevel = 100;
            charging = false;
            break;
        case 6:
            batteryLevel = 75;
            charging = false;
            break;
        case 4:
            batteryLevel = 50;
            charging = false;
            break;
        case 2:
            batteryLevel = 25;
            charging = false;
            break;
        default:
            batteryLevel = -1;
            charging = false;
            break;
        }

    } else {
        qDebug() << "Failed to read battery data.";
        batteryLevel = -1;
        charging = false;
    }

    hid_close(controller);
    hid_exit();
}

QString getBatteryStatus(int batteryLevel) {
    QString variant;

    switch (batteryLevel) {
    case 100:
        variant = "excellent";
        break;
    case 75:
        variant = "good";
        break;
    case 50:
        variant = "mid";
        break;
    case 25:
        variant = "low";
        break;
    default:
        variant = "missing";
        break;
    }

    return variant;
}

QIcon Utils::getIcon(bool proconConnected, bool charging, int batteryLevel, QString &status) {
    QString theme = getTheme();

    if (!proconConnected) {
        status = "Pro controller: disconnected.";
        return QIcon(QString(":/icons/battery-missing-%1.png").arg(theme));
    }

    if (charging) {
        status = "Pro controller: charging.";
        qDebug() << QString(":/icons/battery-charging-%1.png").arg(theme);
        return QIcon(QString(":/icons/battery-charging-%1.png").arg(theme));
    } else {
        QString variant = getBatteryStatus(batteryLevel);
        status = QString("Pro controller: %1%").arg(batteryLevel);
        qDebug() << QString(":/icons/battery-%1-%2.png").arg(variant, theme);
        return QIcon(QString(":/icons/battery-%1-%2.png").arg(variant, theme));
    }
}

