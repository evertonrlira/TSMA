TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    hostapdconfig.cpp \
    util.cpp \
    beacondata.cpp \
    cryptoadapter.cpp

HEADERS += util.h \
    hostapdconfig.h \
    beacondata.h \
    cryptoadapter.h \
    definitions.h


unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lcrypto++

INCLUDEPATH += $$PWD/../../../../../usr/include/crypto++
DEPENDPATH += $$PWD/../../../../../usr/include/crypto++