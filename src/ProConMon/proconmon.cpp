#include "proconmon.h"
#include "utils.h"
#include "shortcutmanager.h"
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QApplication>

using namespace Utils;
using namespace ShortcutManager;

ProConMon::ProConMon(QWidget *parent)
    : QMainWindow(parent)
    , trayIcon(new QSystemTrayIcon(this))
    , proconConnected(false)
    , batteryLevel(-1)
    , charging(false)
    , updateTimer(new QTimer(this))
    , notificationSent(false)
    , settings("Odizinne", "ProConMon")
{
    createTrayIcon();
    loadSettings();
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

    notificationLow = new QAction("Low battery notification", this);
    notificationLow->setCheckable(true);
    notificationLow->setChecked(true);
    connect(notificationLow, &QAction::triggered, this, &ProConMon::saveSettings);
    trayMenu->addAction(notificationLow);

    startupAction = new QAction("Run at startup", this);
    startupAction->setCheckable(true);
    startupAction->setChecked(isShortcutPresent());
    connect(startupAction, &QAction::triggered, this, &ProConMon::onStartupActionStateChanged);
    trayMenu->addAction(startupAction);

    exitAction = new QAction("Exit", this);
    connect(exitAction, &QAction::triggered, &QApplication::quit);
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->setToolTip("ProConMon");
    trayIcon->show();
}

void ProConMon::updateControllerInfos()
{
    getProControllerInfos(proconConnected, batteryLevel, charging);
    QString status;
    trayIcon->setIcon(getIcon(proconConnected, charging, batteryLevel, status));
    trayIcon->setToolTip(status);

    if (!notificationLow->isChecked()) {
        return;
    }

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

void ProConMon::performFirstUpdate()
{
    updateControllerInfos();
}

void ProConMon::loadSettings()
{
    notificationLow->setChecked(settings.value("notification_low", true).toBool());
}

void ProConMon::saveSettings()
{
    settings.setValue("notification_low", notificationLow->isChecked());
}

void ProConMon::onStartupActionStateChanged()
{
    manageShortcut(startupAction->isChecked());
}
