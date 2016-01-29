#ifndef HOSTAPDCONFIG_H
#define HOSTAPDCONFIG_H

#include <fstream>
#include <list>
#include "util.h" 

using namespace std;

class HostapdConfig
{
    public:
    string Interface;
    string Driver;
    string SSID;
    list<string> VendorElements;
    public:
    HostapdConfig();

    void SetVendorElement(string beaconData);
    void ExportToFile(string filePath);
};

#endif // HOSTAPDCONFIG_H
