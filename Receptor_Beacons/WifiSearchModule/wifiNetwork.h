#ifndef WIFINETWORK_H
#define WIFINETWORK_H

#include <string> 
#include <list>
#include "beacondata.h"

using namespace std;

class WifiNetwork
{
public:
    string SSID;
    list<string> TSMACodedMessages;
    BeaconData Message;
    WifiNetwork();
};

#endif // WIFINETWORK_H
