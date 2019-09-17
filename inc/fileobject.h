#ifndef FILEOBJECT_H
#define FILEOBJECT_H

#include <string>
#include <sys/stat.h>

class FileObject {
 public:
    FileObject(std::string file);
    bool openFile();
    bool getLine(std::string &line);    
    void closeFile();
    std::string getFileName();
    void setRetryTime(long time);
    long getRetryTime();
    bool resume();
 private:
    size_t read();
    bool readMore();
    long retry;    
    std::string fileName;    
    FILE *theFile;
    static const size_t BUFFER_SIZE = 4096 * 4; // about half the L1
    char buffer[BUFFER_SIZE];
    size_t charsRead;
    int index;
    std::string remainingLine;
    int fileDescriptor;
    ino_t inode;
    bool isOpen;
    int lastLineIndex;
};
#endif
