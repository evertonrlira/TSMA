#include "trafficsign.h" 

TrafficSign::TrafficSign(string signType, string signDescription)
{
    SignType = signType;
    SignDescription = signDescription;
}

TrafficSign TrafficSign::Parse(string tsmaMessage) {
    TrafficSign ts("UNK", "Erro ao Recuperar");

    try {
        string signTypeHexaStr = tsmaMessage.substr(8+8+16+16+8+8+8, 6);
        //Util::WriteToFile("parseSignTypeHexa.txt", signTypeHexaStr.c_str());
        string signDescriptionHexaStr = tsmaMessage.substr(8+8+16+16+8+8+8+8, 255);
        //Util::WriteToFile("parseSignDescHexa.txt", signDescriptionHexaStr.c_str());

        string signType = Util::HexToString(signTypeHexaStr);
        //Util::WriteToFile("parseSignType.txt", signType.c_str());
        string signDescription = Util::HexToString(signDescriptionHexaStr);
        //Util::WriteToFile("parseSignDesc.txt", signDescription.c_str());

        ts.SignType = signType;
        ts.SignDescription = signDescription;
    }
    catch (...) { }
    return ts;
}
