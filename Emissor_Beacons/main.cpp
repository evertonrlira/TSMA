#include <iostream> 
#include <unistd.h>
#include "hostapdconfig.h"
#include "beacondata.h"
#include "util.h"
#include "definitions.h"

using namespace std;

int main(int argc, char *argv[])
{
    static string outputFileName = "hostapd.conf";
    bool useEncryption = true;

    char c;
    // SignType ('R07')
    string signType = "";
    // Sign Duration (seconds)
    uint32_t signTTL = 0;
    // Sign Range (mt)
    uint8_t signRange = 0;

/*
    while ((c = getopt (argc, argv, "s:t:r:")) != -1)
    {
        switch(c)
        {
            case 's':
                signType = string(optarg);
                break;
            case 't':
                signTTL = (uint32_t)atol(optarg);
                break;
            case 'r':
                signRange = (uint8_t)atoi(optarg);
                break;
            case '?':
                std::cerr << "\nUsage: " << argv[0] << " [Options]\n\
\t-s <SignType ('R07')>\n\t-t <Sign Duration (seconds)>\n\t-r <Sign Range (mt)>\n";
                exit(1);
            default:
                std::cerr << "Usage: ./" << argv[0] << "[Options]\n\
\t-s <SignType ('R07')>\n\t-t <Sign Duration (seconds)>\n\t-r <Sign Range (mt)>\n";
        }
    }
*/

    //string messageToTransmit = "Teste de Transmissao";
    if (argc > 1) {
        std::ostringstream os;
        os << argv[1];
	signType = os.str();
    }
    //    if (argc > 2) {
    //        for(int argIdx = 2; argIdx < argc; ++argIdx) {
    //            os << ' ' << argv[argIdx];
    //        }
    //    }
    //    messageToTransmit = os.str();
    //}
    Definitions defs;

    HostapdConfig* configFile = new HostapdConfig();

    BeaconData* beaconMessage = new BeaconData();

    if (signType.empty())
        signType = "R21";

    if (beaconMessage->ParseSignType(signType)) {
        uint32_t signID = beaconMessage->ParseSignType(signType);
        beaconMessage->SignTypeID = signID;
        const char* signDescription = defs.GetTrafficSignDescription(signType);
        string signDescStr(signDescription);
        beaconMessage->SignDescription = signDescStr;
    }

    if (signTTL != 0)
        beaconMessage->TTL = signTTL;

    if (signRange != 0)
        beaconMessage->Range = signRange;

    configFile->SSID.append(signType);

    string message;
    if (useEncryption)
        message = beaconMessage->GenerateEncryptedMessage();
    else
        message = beaconMessage->GenerateRawMessage();


    configFile->SetVendorElement(message);

    configFile->ExportToFile(outputFileName);

    cout << configFile->VendorElements.back() << endl;
    return 0;
}

