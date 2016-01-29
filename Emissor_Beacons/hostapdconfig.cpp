#include "hostapdconfig.h" 

HostapdConfig::HostapdConfig()
{
    Interface = "wlan0";
    Driver = "nl80211";
    SSID = "Stuffed";
}

void HostapdConfig::SetVendorElement(string beaconData) {
    //string stuffedMessageHexaIdentifier = "56afc19d";

    //string messageInHexa = "dd";

    //int messageLength = messageToTransmit.length() + 4;
    //std::ostringstream os;
    //if (messageLength <= 16)
    //    os << "0";
    //os << std::hex << messageLength;
    //messageInHexa.append(os.str());
    //messageInHexa.append(stuffedMessageHexaIdentifier);

    //messageInHexa.append(Util::StringToHex(messageToTransmit));

    // TODO: Pending adaptation for messages bigger than 255-chars
    VendorElements.push_back(beaconData);
}

void HostapdConfig::ExportToFile(string filePath)
{
    ofstream configFile(filePath.c_str(), ios::trunc);

    configFile << "##### hostapd configuration file ##############################################" << endl;
    configFile << "# AP netdevice name" << endl;
    configFile << "interface=" << Interface.c_str() << endl << endl;

    configFile << "# Driver interface type (hostap/wired/madwifi/test/none/nl80211/bsd)" << endl;
    configFile << "driver=" << Driver.c_str() << endl << endl;

    configFile << "# SSID to be used in IEEE 802.11 management frames" << endl;
    configFile << "ssid=" << SSID.c_str() << endl << endl;

    configFile << "# Operation mode (a = IEEE 802.11a, b = IEEE 802.11b, g = IEEE 802.11g, ad = IEEE 802.11ad (60 GHz)" << endl;
    configFile << "hw_mode=b" << endl << endl;

    configFile << "# Channel number (IEEE 802.11)" << endl;
    configFile << "channel=3" << endl << endl;

    configFile << "# Beacon interval in kus (1.024 ms) (default: 100; range 15..65535)" << endl;
    configFile << "beacon_int=15" << endl << endl;

    configFile << "# Additional vendor specfic elements for Beacon and Probe Response frames" << endl;
    for (list<string>::const_iterator msgIter = VendorElements.begin(); msgIter != VendorElements.end(); ++msgIter) {
        string vendorElement = *msgIter;
        configFile << "vendor_elements=" << vendorElement.c_str() << endl;
    }

    configFile.close();
    return;
}
