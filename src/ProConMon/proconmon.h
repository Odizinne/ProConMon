#ifndef PROCONMON_H
#define PROCONMON_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QSettings>

class ProConMon : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProConMon(QWidget *parent = nullptr);
    ~ProConMon();
    void performFirstUpdate();

private slots:
    void updateControllerInfos();
    void onStartupActionStateChanged();

private:
    QSystemTrayIcon *trayIcon;
    bool proconConnected;
    int batteryLevel;
    bool charging;
    QTimer *updateTimer;
    QAction *exitAction;
    QAction *notificationLow;
    QAction *startupAction;
    bool notificationSent;
    QSettings settings;
    void createTrayIcon();
    void sendNotification(const QString &title, const QString &message, const QIcon &icon, int duration);
    void loadSettings();
    void saveSettings();
};

#endif // PROCONMON_H
