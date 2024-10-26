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
    createTrayIcon();
    connect(updateTimer, &QTimer::timeout, this, &ProConMon::updateControllerInfos);
    updateTimer->start(5000);
}

ProConMon::~ProConMon()
{
    delete trayIcon;
    delete updateTimer;
}

void ProConMon::createTrayIcon()
{
    trayIcon->setIcon(QIcon(":/icons/icon.png"));
    QMenu *trayMenu = new QMenu(this);

    exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, &QApplication::quit);
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->setToolTip("ProConMon");
    trayIcon->show();
}

void ProConMon::updateControllerInfos() {
    getProControllerInfos(proconConnected, batteryLevel, charging);
    QString status;
    trayIcon->setIcon(getIcon(proconConnected, charging, batteryLevel, status));
    trayIcon->setToolTip(status);

    if (proconConnected && !notificationSent && batteryLevel <= 25) {
        sendNotification("Low battery", QString("Pro controller has only 25% battery left."), QIcon(":/icons/icon.png"), 5000);
        notificationSent = true;
    } else if (proconConnected && notificationSent && batteryLevel > 25) {
        notificationSent = false;
    }
}

void ProConMon::sendNotification(const QString &title, const QString &message, const QIcon &icon, int duration)
{
    trayIcon->showMessage(title, message, icon, duration);
}

void ProConMon::performFirstUpdate() {
    updateControllerInfos();
}
