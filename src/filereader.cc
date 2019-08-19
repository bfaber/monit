#include <cstring>
#include <cassert>
#include "filereader.h"
#include "util.h"

FileReader::FileReader(std::string fileName) : fileName(fileName),
					       remainder(nullptr),
					       remSize(0) {
    logStream.open(fileName, std::ios_base::in);
}



void FileReader::getByLine(std::vector<std::string> &lines) {
    if(logStream.is_open()) {
	char buffer[PER_LINE_BUFFER_SIZE];

	while(logStream.getline(buffer, PER_LINE_BUFFER_SIZE)) {
	    lines.push_back(buffer);
	}
    }
}

/**
 *  Read in bufferSize from the file.
 *  Parse into n lines.
 *  Left over buffer goes on heap in remainder.
 *  Remainder is processed first next invocation.
 *  If remainder gets bigger than buffer, process only remainder.
 *  Once end of file is reached, process only remainder
 *   n lines at a time. 
 *
 */
bool FileReader::getNextNLines(std::vector<std::string> &lines, int n) {
    bool remaining = false;

    if(logStream.eof() && remSize == 0)
	return false;

    if(logStream.is_open()) {
	
	size_t bufferSize  = n * PER_LINE_BUFFER_SIZE;
	size_t readBufferSize = bufferSize - remSize;
	char   buffer[bufferSize];

	
	assert(remSize < bufferSize);
	// remSize might be bigger than bufferSize, so shore it up
	if(remSize >= bufferSize) {	    
	    memcpy(buffer, &remainder[0], bufferSize);
	    remSize -= bufferSize;
	    readBufferSize = 0;  // don't read file
	} else {
	    memcpy(buffer, &remainder[0], remSize);
	    remSize = 0;
	    delete remainder;
	    remainder = nullptr;
	}

	if(logStream.eof()) {
	    // dont try reading more from the file,
	    // and only parse into lines the remainder.
	    printf("logstream EOF!!!\n");
	    bufferSize = remSize;
	} else {
	    // still reading from the stream,
	    // read in the buffer minus the remainder
	    printf("reading logstream! buffersize: %zu\n", bufferSize);
	    printf("remsize %d\n", remSize);
	    logStream.read(&buffer[remSize], readBufferSize);
	}
	// parse the buffer into lines delim \n
	int lineCt = 0;
	std::string line;
	int i = 0;
	for(; i < bufferSize; i++) {
		    
	    line += buffer[i];
		
	    if(buffer[i] == '\n') {
		lines.push_back(line);
		line = std::string();
		lineCt++;
		if(lineCt == n) {
		    remaining = true;
		    i++; // push i past last \n
		    break;
		}
	    }
	}

	if(remaining) {
	    delete remainder;

	    remSize = bufferSize - i - 1;
	    remainder = new char[remSize];
	    memcpy(remainder, &buffer[i], remSize);
	}
    }
    return true;
}

bool FileReader::getNextNLines2(std::vector<std::string> &lines, int n) {
    if( logStream.eof() && remSize == 0 ) {
	return false;
    }
    
    size_t bufferSize = n * PER_LINE_BUFFER_SIZE;
    size_t readBufferSize = bufferSize - remSize;
    
    char buffer[bufferSize];

    memcpy(buffer, &remainder[0], remSize);

    logStream.read(&buffer[remSize], readBufferSize);

    bool processRemaining = false;
    if( logStream.eof() ) {
	processRemaining = true;
	// gcount is number of characters read
	bufferSize = remSize + logStream.gcount();
    }

    bool remaining = false;
    int lineCt = 0;
    std::string line;
    int i = 0;
    for( ; i < bufferSize; i++ ) {

	line += buffer[i];

	if( buffer[i] == '\n' ) {
	    lines.push_back(line);
	    line = std::string();
	    lineCt++;
	    if( lineCt == n && !processRemaining ) {
		remaining = true;
		break;
	    }	    
	}
    }

    if( remaining ) {
	delete remainder;
	remSize = bufferSize - i;
	remainder = new char[remSize];
	memcpy(remainder, &buffer[i+1], remSize);
    }

    if( processRemaining ) {
	remSize = 0;	
    } 
    return true;
    
}

void FileReader::close() {
    logStream.close();
}

void FileReader::readWholeFile() {
    std::ifstream filestream;
    filestream.open(fileName, std::ios_base::in);
    size_t bufferSize = 4096;
    char buffer[bufferSize];
    long t0 = Util::timeMs();
    std::vector<std::string> chunks;
    // 24ms to read file
    while(filestream.read(buffer, bufferSize)) {
	std::string chunk = buffer;
	chunks.push_back(chunk);
    } // 69ms to parse into chunks
    filestream.close();
    std::vector<char*> lines;
    lines.reserve(50787); // no help perf
    char* line = new char[1024];
    int ct = 0;
    for(auto chunk : chunks) {
	for(auto ch : chunk) {
	    if(ch == '\n') {
		lines.push_back(line);
		line = new char[1024];
		ct = 0;
	    }
	    line[ct] = ch;
	    ct++;
	}
    } //490ms using std::string for lines,
    // 277ms using char[1024] arrays on heap.
    long t1 = Util::timeMs();
    printf("FileReader DT: %ldms\n", (t1 - t0));

}















