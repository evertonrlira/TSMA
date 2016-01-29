#include "cryptoadapter.h" 

CryptoAdapter::CryptoAdapter()
{
}

void CryptoAdapter::RSAGenerateKeyPair(string privateKeyFilepath, string publicKeyFilepath) {
    //CryptoPP::SecByteBlock seed(kKeyLength/12);
    //CryptoPP::OS_GenerateRandomBlock(false, seed, seed.size());
    //CryptoPP::RandomPool randPool;
    //randPool.IncorporateEntropy(seed, seed.size());
    CryptoPP::AutoSeededRandomPool randPool;

    CryptoPP::RSAES_OAEP_SHA_Decryptor priv(randPool, kKeyLength*1);
    CryptoPP::HexEncoder privFile(new CryptoPP::FileSink(privateKeyFilepath.c_str()));
    priv.DEREncode(privFile);
    privFile.MessageEnd();

    CryptoPP::RSAES_OAEP_SHA_Encryptor pub(priv);
    CryptoPP::HexEncoder pubFile(new CryptoPP::FileSink(publicKeyFilepath.c_str()));
    pub.DEREncode(pubFile);
    pubFile.MessageEnd();
}

string CryptoAdapter::RSAEncrypt(string message, string publicKeyFilepath) {
    string result;

    CryptoPP::AutoSeededRandomPool randPool;

    CryptoPP::FileSource pubFile(publicKeyFilepath.c_str(), true, new CryptoPP::HexDecoder);
    CryptoPP::RSAES_OAEP_SHA_Encryptor pub(pubFile);
    CryptoPP::StringSource(message.c_str(), true, new CryptoPP::PK_EncryptorFilter(randPool, pub, new CryptoPP::HexEncoder(new CryptoPP::StringSink(result))));
    return result;
}

string CryptoAdapter::RSADecrypt(string encryptedMessage, string privateKeyFilepath) {
    CryptoPP::FileSource privFile(privateKeyFilepath.c_str(), true, new CryptoPP::HexDecoder);
    CryptoPP::RSAES_OAEP_SHA_Decryptor priv(privFile);

    CryptoPP::AutoSeededRandomPool randPool;

    string result;
    CryptoPP::StringSource(encryptedMessage.c_str(), true, new CryptoPP::HexDecoder(new CryptoPP::PK_DecryptorFilter(randPool, priv, new CryptoPP::StringSink(result))));
    return result;
}

string CryptoAdapter::RSAGetPrivateKeyFilepath(uint8_t countryID, uint32_t locationID) {
    return "PrivateKey.keyfile";
}

string CryptoAdapter::RSAGetPublicKeyFilepath(uint8_t countryID, uint32_t locationID) {
    return "PublicKey.keyfile";
}
