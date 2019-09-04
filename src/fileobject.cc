#include "fileobject.h"

//const size_t BUFFER_SIZE = 4096; // would be nice to hook this directly to the OS, query page size

FileObject::FileObject(std::string filen) : retry(0),
					    fileName(filen),
					    theFile(nullptr),
					    charsRead(0),
					    index(0),
					    fileDescriptor(0) {

}

bool FileObject::openFile() {
    charsRead = 0;
    index = 0;
    theFile = fopen(fileName.c_str(), "r");
    fileDescriptor = fileno(theFile);
    if(theFile) {
	read();
	return true;
    }
    return false;
}

void FileObject::setRetryTime(long time) {
    retry = time;
}

long FileObject::getRetryTime() {
    return retry;
}

std::string FileObject::getFileName() {
    return fileName;
}

size_t FileObject::read() {
    //printf("read called\n");
    charsRead = fread(buffer, sizeof(char), BUFFER_SIZE, theFile);
    //printf("chars read %lu\n", charsRead);
    //buffer[charsRead] = '\0';
    bool eof = false;
    if( charsRead == 0 ) {
	eof = true;	
    }
    return eof;
}

bool FileObject::readMore() {
    charsRead = 0;
    index = 0;
    bool eof = read();
    if( eof ) {	
	// so now check to see about log rotation
	FILE* file = fopen(fileName.c_str(), "r");
	int aFileDescriptor = fileno(theFile);
	if( aFileDescriptor != fileDescriptor ) {
	    printf("file descriptors don't match!\n");
	    closeFile();
	    fileDescriptor = aFileDescriptor;
	    theFile = file;
	    // attempt read again
	    eof = read();
	} 
    }
    if( !eof ) {
	return true;
    } else {
	return false;
    }
}

/**
 *  Resume has to make another read, kind of kickstarting
 * our system here
 */
bool FileObject::resume() {
    clearerr(theFile); // clear the eof flag in the stream
    return readMore();
}

bool FileObject::getLine(std::string &line) {

    if( remainingLine.size() > 0 ) {
	line += remainingLine;
    }

    bool newline = false;
    bool endOfFile = false;
    while( index < charsRead ) {
	// hit the newline return it
	if ( buffer[index] == '\n' ) {
	    newline = true;
	    index++; // get past newline
	    //	    printf("newline\n");
	    break;
	}
	
	line += buffer[index];
	index++;	

	

	// hit the buffer, check if eof
	if( index == charsRead ) {
	    if(buffer[index] == '\n') {
		newline = true;
	    }
	    endOfFile = !readMore();
	    if( endOfFile ) {
		printf("end of buffer, end of file\n");
		break;
	    }
	}
    }

    if ( newline ) {
	// got a line, check if also end of buffer; eof

	if( index == charsRead ) { // goldilocks
	    endOfFile = !readMore();
	    if( endOfFile ) {
		return true; // still return true here,
		// need to process that last line.
		// next call to getline will fail.
	    }
	}
	return true;
    } else {	// hit eof
	printf("def eof!\n");
	if( line.size() > 0 ) {
	    remainingLine = line;
	    printf("actually hit remainingLine\n");
	}
	return false;
    }
}

void FileObject::closeFile() {
    fclose(theFile);
    theFile = nullptr;
    fileDescriptor = 0;
}
