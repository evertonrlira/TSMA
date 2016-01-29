#ifndef CRYPTOADAPTER_H
#define CRYPTOADAPTER_H

#include <string>
#include <stdint.h>
#include "rsa.h"
#include "base32.h"
#include "base64.h"
#include "files.h"
#include "osrng.h"
#include "queue.h"
#include "secblock.h"
#include "hex.h"
#include "util.h"

using namespace std;

class CryptoAdapter
{
    public:
        const static unsigned int kKeyLength = 1536;
        CryptoAdapter();
        static string RSAGetPrivateKeyFilepath(uint8_t countryID, uint32_t locationID);
        static string RSAGetPublicKeyFilepath(uint8_t countryID, uint32_t locationID);
        static void RSAGenerateKeyPair(string privateKeyFilepath, string publicKeyFilepath);
        static string RSAEncrypt(string message, string publicKeyFilepath);
        static string RSADecrypt(string encryptedMessage, string privateKeyFilepath);
};

#endif // CRYPTOADAPTER_H
