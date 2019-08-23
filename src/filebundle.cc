#include "filebundle.h"

FileBundle::FileBundle() : charCt(0),
			   retry(0) {}

void FileBundle::addBundle(std::string fname, MatchBundle *mb) {
    filename = fname;
    bundles.push_back(mb);
}

void FileBundle::addCharCount(unsigned long cc) {
    charCt += cc;
}

unsigned long FileBundle::getCharCount() {
    return charCt;
}

void FileBundle::setRetryTime(long rt) {
    retry = rt;
}

long FileBundle::getRetryTime() {
    return retry;
}

std::vector<MatchBundle*>& FileBundle::getBundles() {
    return bundles;
}

