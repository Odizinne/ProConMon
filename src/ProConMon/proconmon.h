#ifndef PROCONMON_H
#define PROCONMON_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class ProConMon : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProConMon(QWidget *parent = nullptr);
    ~ProConMon();

private slots:
    void updateControllerInfos();

private:
    QSystemTrayIcon *trayIcon;  // System tray icon
    bool proconConnected;        // Connection status
    int batteryLevel;           // Battery level percentage
    bool charging;              // Charging status
    QTimer *updateTimer;        // Timer for periodic updates
    QAction *exitAction;        // Exit action for tray menu
    bool notificationSent;      // Notification state

    void createTrayIcon();      // Method to create the system tray icon
    void sendNotification(const QString &title, const QString &message, const QIcon &icon, int duration); // Method to send notifications
};

#endif // PROCONMON_H
