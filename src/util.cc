	
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "util.h"

std::vector<std::string> Util::splitString(std::string s, char c) {
    std::stringstream sstream(s);
    std::vector<std::string> ss;
    std::string item;

    while (std::getline(sstream, item, c))
	ss.push_back(item);

    return ss;
}
