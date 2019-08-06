#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <iostream>
#include <fstream>

class FileReader {
 public:
    FileReader(std::string fileName);
    void getByLine(std::vector<std::string> &lines);
    bool getNextNLines(std::vector<std::string> &lines, int n);
    bool getNextNLines2(std::vector<std::string> &lines, int n);

 private:
    std::streamsize PER_LINE_BUFFER_SIZE = 1024;

    std::string fileName;
    int filesize;
    std::ifstream logStream;
    char* remainder;
    uint remSize;
};
#endif
