#ifndef BEACONDATA_H
#define BEACONDATA_H

#include <stdint.h>  
#include <list>
#include <algorithm>
#include "util.h"
#include "cryptoadapter.h"

class BeaconData
{
    public:

    uint8_t Version;        // FF - 0 <-> 255
    uint16_t BeaconType;    // FFFF
    uint8_t CountryID;      // FF - 0 <-> 255
    uint32_t LocationID;    // FFFFFFFF
    uint64_t UID;           // FFFFFFFFFFFFFFFF
    uint64_t TimeStamp;     // FFFFFFFFFFFFFFFF
    uint32_t GeoLatitude;   // FFFFFFFF
    uint32_t GeoLongitude;  // FFFFFFFF
    uint32_t TTL;           // FFFFFFFF
    uint32_t SignTypeID;    // 24B FF
    uint8_t Range;          // FF - 0 <-> 255
    string SignDescription;

    public:
    BeaconData();
    string GenerateRawMessage();
    string GenerateEncryptedMessage();
};

#endif // BEACONDATA_H
