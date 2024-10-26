#include "proconmon.h"
#include "utils.h"
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QApplication>

using namespace Utils;

ProConMon::ProConMon(QWidget *parent)
    : QMainWindow(parent),
    trayIcon(new QSystemTrayIcon(this)),
    proconConnected(false),
    batteryLevel(-1),
    charging(false),
    updateTimer(new QTimer(this)),
    notificationSent(false)
{
    createTrayIcon();  // Create the tray icon
    updateControllerInfos();  // Initial update

    // Connect the timer's timeout signal to the update function
    connect(updateTimer, &QTimer::timeout, this, &ProConMon::updateControllerInfos);
    updateTimer->start(5000);  // Update every 5 seconds
}

ProConMon::~ProConMon()
{
    // Clean up resources
    delete trayIcon;  // Delete the tray icon
    delete updateTimer;  // Delete the timer
}

void ProConMon::createTrayIcon()
{
    trayIcon->setIcon(QIcon(":/icons/icon.png"));  // Set the icon
    QMenu *trayMenu = new QMenu(this);  // Create context menu

    exitAction = new QAction("Exit", this);  // Exit action
    connect(exitAction, &QAction::triggered, &QApplication::quit);  // Connect to exit application
    trayMenu->addAction(exitAction);  // Add action to menu

    trayIcon->setContextMenu(trayMenu);  // Set the context menu
    trayIcon->setToolTip("ProConMon");  // Set tooltip for the tray icon
    trayIcon->show();  // Show the tray icon
}

void ProConMon::updateControllerInfos() {
    getProControllerInfos(proconConnected, batteryLevel, charging);  // Get the current controller info
    QString status;
    trayIcon->setIcon(getIcon(proconConnected, charging, batteryLevel, status));  // Update the icon based on status
    trayIcon->setToolTip(status);  // Update the tooltip

    // Notification for low battery
    if (proconConnected && !notificationSent && batteryLevel <= 25) {
        sendNotification("Low battery", QString("Pro controller has only 25% battery left."), QIcon(":/icons/icon.png"), 5000);
        notificationSent = true;  // Mark notification as sent
    } else if (proconConnected && notificationSent && batteryLevel > 25) {
        notificationSent = false;  // Reset notification state if battery level is above 25%
    }
}

void ProConMon::sendNotification(const QString &title, const QString &message, const QIcon &icon, int duration)
{
    trayIcon->showMessage(title, message, icon, duration);  // Display notification
}
