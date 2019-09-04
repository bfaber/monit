#ifndef FILEBUNDLE_H
#define FILEBUNDLE_H

#include <string>
#include <vector>

#include "matchbundle.h"
#include "fileobject.h"

class FileBundle {
 public:
    FileBundle(std::string filename);
    void addBundle(MatchBundle *mb);
    std::vector<MatchBundle*>& getBundles();
    
    unsigned long getCharCount();
    void addCharCount(unsigned long cc);
    FileObject& getFileHandler();
    
 private:
    FileObject fileObj;
    std::string filename;
    std::vector<MatchBundle*> bundles;
    unsigned long charCt;

};
#endif
