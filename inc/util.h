#ifndef UTIL_H
#define UTIL_H

#include <vector>


class Util {

 public:
    static std::vector<std::string> splitString(std::string s, char c);
    static long timeMs();
    static void getLinesFromBuffer(char* buffer, std::vector<std::string> &lines);
    static void sleepMs(unsigned long ms);
};
#endif
