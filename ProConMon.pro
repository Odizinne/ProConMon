QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH +=                          \
    dependencies/hidapi-win/include/    \
    src/ProconMon/                      \
    src/Utils/                          \

SOURCES +=                              \
    src/main.cpp                        \
    src/ProConMon/proconmon.cpp         \
    src/Utils/utils.cpp                 \

HEADERS +=                              \
    src/ProConMon/proconmon.h           \
    src/Utils/utils.h                   \

RESOURCES +=                            \
    src/resources/resources.qrc         \

RC_FILE = src/Resources/appicon.rc

LIBS += -L"$$PWD/dependencies/hidapi-win/x64/" -lhidapi

