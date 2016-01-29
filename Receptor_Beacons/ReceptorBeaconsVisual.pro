TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    WifiSearchModule/util.cpp \
    WifiSearchModule/wifiDevice.cpp \
    WifiSearchModule/wifiNetwork.cpp \
    WifiSearchModule/iwlist-SRC/iwlib.c \
    WifiSearchModule/iwlist-SRC/iwlistMinimal.c \
    stubWifiScanModule.cpp \
    realWifiScanModule.cpp \
    WifiSearchModule/beacondata.cpp \
    WifiSearchModule/cryptoadapter.cpp \
    trafficsign.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    WifiSearchModule/iwlist-SRC/iwlib-private.h \
    WifiSearchModule/iwlist-SRC/iwlib.h \
    WifiSearchModule/iwlist-SRC/wireless.h \
    WifiSearchModule/util.h \
    WifiSearchModule/wifiDevice.h \
    WifiSearchModule/wifiNetwork.h \
    stubWifiScanModule.h \
    realWifiScanModule.h \
    WifiSearchModule/beacondata.h \
    WifiSearchModule/cryptoadapter.h \
    trafficsign.h

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lcrypto++

INCLUDEPATH += $$PWD/../../../../usr/include/crypto++
DEPENDPATH += $$PWD/../../../../usr/include/crypto++
