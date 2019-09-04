#include "filebundle.h"

FileBundle::FileBundle(std::string filen) : fileObj(filen),
					    filename(filen),
					    charCt(0) {}


void FileBundle::addBundle(MatchBundle *mb) {
    bundles.push_back(mb);
}

void FileBundle::addCharCount(unsigned long cc) {
    charCt += cc;
}

unsigned long FileBundle::getCharCount() {
    return charCt;
}

std::vector<MatchBundle*>& FileBundle::getBundles() {
    return bundles;
}

FileObject& FileBundle::getFileHandler() {
    return fileObj;
}


