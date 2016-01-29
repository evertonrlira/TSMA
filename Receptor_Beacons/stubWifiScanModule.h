#ifndef STUBWIFISCANMODULE_H
#define STUBWIFISCANMODULE_H

#include <string>
#include <QQuickView>
#include <unistd.h>

using namespace std;

class StubWifiScanModule : public QObject
{
    Q_OBJECT

    public:
        string LastReceivedMessage;

        StubWifiScanModule();
        void Run();

    public slots:
        signals:
            void trafficSignReceived(QVariant signType, QVariant signMessage);
};


#endif // STUBWIFISCANMODULE_H
