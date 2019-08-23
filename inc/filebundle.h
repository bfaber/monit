#ifndef FILEBUNDLE_H
#define FILEBUNDLE_H

#include <string>
#include <vector>

#include "matchbundle.h"

class FileBundle {
 public:
    FileBundle();
    void addBundle(std::string filename, MatchBundle *mb);
    std::vector<MatchBundle*>& getBundles();
    
    unsigned long getCharCount();
    void addCharCount(unsigned long cc);
    void setRetryTime(long time);
    long getRetryTime();

    
 private:
    std::string filename;
    std::vector<MatchBundle*> bundles;
    unsigned long charCt;
    long retry;
};
#endif
