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
    void performFirstUpdate();

private slots:
    void updateControllerInfos();

private:
    QSystemTrayIcon *trayIcon;
    bool proconConnected;
    int batteryLevel;
    bool charging;
    QTimer *updateTimer;
    QAction *exitAction;
    bool notificationSent;

    void createTrayIcon();
    void sendNotification(const QString &title, const QString &message, const QIcon &icon, int duration); // Method to send notifications
};

#endif // PROCONMON_H
