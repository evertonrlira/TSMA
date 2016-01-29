#ifndef WIFISCANMODULE_H
#define WIFISCANMODULE_H

#include <string>
#include <QQuickView>
#include "WifiSearchModule/wifiDevice.h"
#include "WifiSearchModule/util.h"
#include "trafficsign.h"
#include <unistd.h>

using namespace std;

class WifiScanModule : public QObject
{
    Q_OBJECT

    public:
        string LastReceivedMessage;

        WifiScanModule();
        void Run();

    public slots:
        signals:
            void trafficSignReceived(QVariant signType, QVariant signMessage);
};

#endif // WIFISCANMODULE_H
