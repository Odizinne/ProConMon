#include "proconmon.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProConMon proconMon;
    proconMon.performFirstUpdate();
    return a.exec();
}
