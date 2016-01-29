#include "realWifiScanModule.h"

WifiScanModule::WifiScanModule()
{
}

void WifiScanModule::Run() {
    WifiDevice wifiDevice("wlan0");
    string lastDisplayedTrafficSignType = "-1";
    string currentlyFoundTrafficSignType = "";
    TrafficSign foundMessage("", "");

    while(true) {
	//Util::WriteToFile("Log.txt", "Went in the loop");

        list<WifiNetwork>* tsmaWifiNetworks = wifiDevice.ScanForTSMAWifiNetworks();

        if (tsmaWifiNetworks->size() == 0) {
            //Util::WriteToFile("Log.txt", "\nFoundNoNetwork");
            currentlyFoundTrafficSignType = "";
            if (currentlyFoundTrafficSignType != lastDisplayedTrafficSignType) {
                emit trafficSignReceived(currentlyFoundTrafficSignType.c_str(), "Nenhuma Sinalizacao Disponivel");
                //Util::WriteToFile("Log.txt", "\nFoundNoNetwork - Emitted Alert");
            }
            lastDisplayedTrafficSignType = currentlyFoundTrafficSignType;
        }
        else {
            //Util::WriteToFile("Log.txt", "\nFoundANetwork");
            WifiNetwork tsmaNetwork = tsmaWifiNetworks->front();
            //Util::WriteToFile("Log1.txt", "\nFoundANetwork - Selected It");
            string tsmaCodedMessage = tsmaNetwork.TSMACodedMessages.front();
            //Util::WriteToFile("LogOriginal.txt", tsmaCodedMessage.c_str());
            string rawPart = tsmaCodedMessage.substr(0, 16);
            //Util::WriteToFile("LogRawPart.txt", rawPart.c_str());
            string codedPart = tsmaCodedMessage.substr(16, 512);
            //Util::WriteToFile("LogCodedPart.txt", codedPart.c_str());
            string privateKeyFilepath = CryptoAdapter::RSAGetPrivateKeyFilepath(0,0);
            //Util::WriteToFile("Log2.txt", privateKeyFilepath.c_str());
            string decrypted = CryptoAdapter::RSADecrypt(codedPart, privateKeyFilepath);
            string tsmaMessage = rawPart.append(decrypted);
            //Util::WriteToFile("LogBeforeParse.txt", tsmaMessage.c_str());
            TrafficSign trafficSign = TrafficSign::Parse(tsmaMessage);
            //Util::WriteToFile("Log4.txt", "\nFoundANetwork - Parsed Message: ");
            //Util::WriteToFile("Log4.txt", trafficSign.SignType.c_str());
            

            currentlyFoundTrafficSignType = trafficSign.SignType;
            if (currentlyFoundTrafficSignType != lastDisplayedTrafficSignType) {
                emit trafficSignReceived(currentlyFoundTrafficSignType.c_str(), trafficSign.SignDescription.c_str());
                //Util::WriteToFile("Log5.txt", "\nFoundANetwork - Emitted Signal");
            }

            lastDisplayedTrafficSignType = currentlyFoundTrafficSignType;
        }


        //if (foundMessages.size() > 0)
        //    foundMessage = foundMessages.front();

        //if (foundMessage.SignType != lastDisplayedTrafficSignType) {
        //    emit trafficSignReceived(foundMessage.SignType.c_str(), foundMessage.SignDescription.c_str());
        //}
    }
}
