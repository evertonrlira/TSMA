#ifndef WIFIDEVICE_H
#define WIFIDEVICE_H 

#include <iostream>
#include <list>
#include <sstream>
//#include <iostream>
//#include <iterator>
#include <iomanip>
#include "iwlist-SRC/iwlib.h"
#include "wifiNetwork.h"
#include "util.h"
#include "trafficsign.h"

using namespace std;

class WifiDevice
{
public:
    string Name;
    //string Driver;
    //string SSID;
    //list<string> VendorElements;
public:
    WifiDevice(string deviceName);


    //list<TrafficSign>* ScanForTSMATrafficSigns();

    list<WifiNetwork>* ScanForTSMAWifiNetworks();

    list<WifiNetwork> ScanForWiFiNetworks(int skfd, char* ifname, char* args[], int count);

    list<string> ScanForMessages();
    list<string> ScanForMessages(string channel);

    //void SetMessage(string messageToTransmit);
    //void ExportToFile(string filePath);
};

int list_available_wifi_cells(string wifiDeviceName);

// Método Adaptado da Duplicação do iwlist.c -> print_scanning_token
inline WifiNetwork*
RetrieveWifiNetworkInfo(struct stream_descr *	stream,	/* Stream of events */
                  struct iw_event *		event,	/* Extracted token */
                  struct iwscan_state *	state,
                  struct iw_range *	iw_range,	/* Range info */
                  int		has_range,
                        WifiNetwork* lastFoundWifiNetwork);

inline list<string> RetrieveStuffedTSMAMessages(unsigned char *	buffer,
                int		buflen);

// Método Adaptado da Duplicação do iwlib.c -> iw_print_stats
void
print_event_quality_signal_level(char *		buffer,
               int		buflen,
               const iwqual *	qual,
               const iwrange *	range,
               int		has_range);

#endif // WIFIDEVICE_H
