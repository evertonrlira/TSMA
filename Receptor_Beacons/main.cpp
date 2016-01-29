#include <QApplication>
#include <QQmlApplicationEngine>
//#include <QtDeclarative/QDeclarativeEngine>
//#include <QtDeclarative/QDeclarativeComponent>
#include <QFuture>
#include <QQuickView>
#include <QtConcurrent/QtConcurrent>
#include <string>
#include <list>
#include "realWifiScanModule.h"

using namespace std;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine (QUrl("qrc:/main.qml"));

    QObject* object = engine.rootObjects().first();
    QObject* receivedSignsView = object->findChild<QObject*>("receivedSignsView");

    WifiScanModule* wifiScan = new WifiScanModule();
    wifiScan->connect(wifiScan, SIGNAL(trafficSignReceived(QVariant,QVariant)), receivedSignsView, SLOT(displaySign(QVariant,QVariant)));

    QtConcurrent::run(wifiScan, &WifiScanModule::Run);

    return app.exec();
}


