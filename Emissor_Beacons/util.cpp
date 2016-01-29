#include "util.h" 

Util::Util()
{
}

string Util::StringToHex(string inputString)
{
    string hexString;

    for(std::string::iterator charIter = inputString.begin(); charIter != inputString.end(); ++charIter) {
        char character = *charIter;
        int equivalentASCII = character;
        std::ostringstream os;
        os << std::hex << equivalentASCII;
        hexString.append(os.str());
    }
    return hexString;
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
string Util::CurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}

bool Util::DoesFileExists(string filepath) {
    std::ifstream infile(filepath.c_str());
    return infile.good();
}

string Util::GetRandomAlphanumericString(unsigned int length) {
    char s[length+1];
    for (int i = 0; i < length; ++i) {
        int randomChar = rand()%(26+26+10);
        if (randomChar < 26)
            s[i] = 'a' + randomChar;
        else if (randomChar < 26+26)
            s[i] = 'A' + randomChar - 26;
        else
            s[i] = '0' + randomChar - 26 - 26;
    }
    s[length] = 0;
    string str(s);
    return str;
}
