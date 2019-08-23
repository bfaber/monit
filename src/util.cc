	
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <sys/time.h>
#include <chrono>
#include <thread>

#include "util.h"

std::vector<std::string> Util::splitString(std::string s, char c) {
    std::stringstream sstream(s);
    std::vector<std::string> ss;
    std::string item;

    while (std::getline(sstream, item, c))
	ss.push_back(item);

    return ss;
}



long Util::timeMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// The tradeoff here is allocating a chunk of memory for strings at once,
// rather than every time we dip into 'getline'
void Util::getLinesFromBuffer(char* buffer, std::vector<std::string> &lines) {
    
    std::stringstream stream(buffer);
    std::string line;
    while(std::getline(stream, line, '\n')) {
	lines.push_back(line);
    }
}

void Util::sleepMs(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
