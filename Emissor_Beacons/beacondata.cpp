#include "beacondata.h" 

BeaconData::BeaconData()
{
    Version = 1;
    BeaconType = 0x56AF; // Fixo
    CountryID = 55;
    LocationID = 3170206; // Código IBGE - Uberlândia
    UID = 885290951;
    TimeStamp = 1420369218; // 04/01/2015 09:00:15 - http://www.timestampconvert.com/
    GeoLatitude = 0x12A336; // http://www.mapcoordinates.net/en
    GeoLongitude = 0x8021E093;
    SignTypeID = 0x5207; // Proibido Ultrapassar R-07
    TTL = 7200; // 2h
    Range = 80;
    SignDescription = "Overtake Forbidden";
}

string BeaconData::GenerateRawMessage()
{
    std::ostringstream os;
    os << std::hex << std::setfill('0');

    os << std::setw(2) << (int)Version;
    os << std::setw(4) << BeaconType;
    os << std::setw(2) << (int)CountryID;
    os << std::setw(8) << LocationID;
    os << std::setw(16) << UID;
    os << std::setw(16) << TimeStamp;
    os << std::setw(8) << GeoLatitude;
    os << std::setw(8) << GeoLongitude;
    os << std::setw(8) << TTL;
    os << std::setw(6) << SignTypeID;
    os << std::setw(2) << (int)Range;

    string fieldsInHexa = os.str();
    string descriptionInHexa = Util::StringToHex(SignDescription);
	std::ostringstream os2;
	os2 << std::hex << std::setfill('0');
    os2 << std::setw(2) << (fieldsInHexa.length() / 2) + descriptionInHexa.length();
    string lengthInHexa = os2.str();

    string messageInHexa;
    messageInHexa.append("dd");
    messageInHexa.append(lengthInHexa);
    messageInHexa.append(fieldsInHexa);
    messageInHexa.append(descriptionInHexa);

	std::transform(messageInHexa.begin(), messageInHexa.end(), messageInHexa.begin(), ::toupper);
    return messageInHexa;
}

string BeaconData::GenerateEncryptedMessage() {
    int lengthInHexa = 0;

    std::ostringstream osRaw;
    osRaw << std::hex << std::setfill('0');
    osRaw << std::setw(2) << (int)Version;
    osRaw << std::setw(4) << BeaconType;
    osRaw << std::setw(2) << (int)CountryID;
    osRaw << std::setw(8) << LocationID;
    string rawSegment = osRaw.str();
    std::transform(rawSegment.begin(), rawSegment.end(), rawSegment.begin(), ::toupper);

    std::ostringstream osCrypto;
    osCrypto << std::hex << std::setfill('0');
    osCrypto << std::setw(16) << UID;
    osCrypto << std::setw(16) << TimeStamp;
    osCrypto << std::setw(8) << GeoLatitude;
    osCrypto << std::setw(8) << GeoLongitude;
    osCrypto << std::setw(8) << TTL;
    osCrypto << std::setw(6) << SignTypeID;
    osCrypto << std::setw(2) << (int)Range;
    string descriptionInHexa = Util::StringToHex(SignDescription);
    string segmentToEncrypt = osCrypto.str().append(descriptionInHexa);
    std::transform(segmentToEncrypt.begin(), segmentToEncrypt.end(), segmentToEncrypt.begin(), ::toupper);

    string publicKeyFilepath = CryptoAdapter::RSAGetPublicKeyFilepath(CountryID, LocationID);
    string privateKeyFilepath = CryptoAdapter::RSAGetPrivateKeyFilepath(CountryID, LocationID);
    if (!Util::DoesFileExists(privateKeyFilepath) || !Util::DoesFileExists(publicKeyFilepath))
        CryptoAdapter::RSAGenerateKeyPair(privateKeyFilepath, publicKeyFilepath);
    string encryptedSegment = CryptoAdapter::RSAEncrypt(segmentToEncrypt, publicKeyFilepath);

    int esLength = encryptedSegment.length();

    int messageLength = (rawSegment.length() + encryptedSegment.length()) / 2;
    std::ostringstream osLength;
    osLength << std::hex << std::setfill('0');
    osLength << std::setw(2) << messageLength;
    string lengthSegment = osLength.str();
    std::transform(lengthSegment.begin(), lengthSegment.end(), lengthSegment.begin(), ::toupper);

    string messageInHexa;
    messageInHexa.append("DD");
    messageInHexa.append(lengthSegment);
    messageInHexa.append(rawSegment);
    messageInHexa.append(encryptedSegment);

    return messageInHexa;
    //string decryptedSegment = CryptoAdapter::RSADecrypt(encryptedSegment, privateKeyFilepath);
}

uint32_t BeaconData::ParseSignType(string signType) {
    uint32_t signTypeID = 0;

    if (signType[0] == 'R' || signType[0] == 'r')
        signType[0] = (char)52;
    signType[1] = (char)(signType[1] - 18);
    signType[2] = (char)(signType[2] - 18);

    if (signType.size() == 3) {
        signTypeID += ((signType[0]/10) * 1048576);
        signTypeID += ((signType[0]%10) * 65536);
        signTypeID += ((signType[1]/10) * 4096);
        signTypeID += ((signType[1]%10) * 256);
        signTypeID += ((signType[2]/10) * 16);
        signTypeID += ((signType[2]%10) * 1);
    }
/*
    if (signType[0] == 'R' || signType[0] == 'r') {
        signTypeID += 0x520000;
        string id = signType.substr(1, 3);
        if (std::stoi(id) && std::stoi(id) <= 255)
            signTypeID += std::stoi(id);
    }*/
    return signTypeID; // R07 = 20999
}
