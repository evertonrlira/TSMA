#ifndef TRAFFICSIGN_H
#define TRAFFICSIGN_H

#include <string>
#include "WifiSearchModule/util.h" 

using namespace std;

class TrafficSign
{
public:
    string SignType;
    string SignDescription;

public:
    TrafficSign(string signType, string signDescription);
    static TrafficSign Parse(string tsmaMessage);
};

#endif // TRAFFICSIGN_H
