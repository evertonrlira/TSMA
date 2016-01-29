#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdint.h> 

using namespace std;

class Util
{
public:
    Util();
    static string StringToHex(string inputString);
    static string CurrentDateTime();
    static string GetRandomAlphanumericString(unsigned int length);
    static bool DoesFileExists(string filepath);
};

#endif // UTIL_H
