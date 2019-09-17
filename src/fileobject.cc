#include <errno.h>

#include "fileobject.h"


//const size_t BUFFER_SIZE = 4096; // would be nice to hook this directly to the OS, query page size

FileObject::FileObject(std::string filen) : retry(0),
					    fileName(filen),
					    theFile(nullptr),
					    charsRead(0),
					    index(0),
					    fileDescriptor(0),
					    isOpen(false),
					    lastLineIndex(0) {

    printf("FileObject ctor!\n");
}

bool FileObject::openFile() {
    if( isOpen ) {
	return true;
    }
    charsRead = 0;
    index = 0;
    theFile = fopen(fileName.c_str(), "r");
    if(theFile) {
	fileDescriptor = fileno(theFile);
	struct stat statBuf;
	if( fstat(fileDescriptor, &statBuf) < 0 ) {
	    printf("Couldnt fstat file on open file. %s\n", fileName.c_str());
	    printf("Can't fstat file %s\n", strerror(errno));
	    return false;
	}
	inode = statBuf.st_ino;
    }
    if(theFile) {
	//read();
	isOpen = true;
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

    lastLineIndex = 0;
    charsRead = 0;
    index = 0;
    bool eof = read();
    if( eof ) {	
	struct stat statBuf;
	if( stat(fileName.c_str(), &statBuf) < 0 ) {
	    printf("Can't fstat file %s\n", fileName.c_str());
	    printf("Can't fstat file %s\n", strerror(errno));
	    return false;
	}
	
	ino_t aInode = statBuf.st_ino;
	if( aInode != inode ) {
	    printf("Inodes don't match!\n");
	    inode = aInode;
	    closeFile();
	    theFile = fopen(fileName.c_str(), "r");
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
 *  Resume has to make a read, kind of kickstarting
 * our system here
 */
bool FileObject::resume() {
    clearerr(theFile); // clear the eof flag in the stream
    return readMore();
}

bool FileObject::getLine(std::string &line) {
    bool eof = false;
    bool newline = false;

    line.clear();

    line.append(remainingLine);
    
    while( index < charsRead ) {
	if( buffer[index] == '\n' ) {
	    line.append(&buffer[lastLineIndex], index - lastLineIndex);
	    //printf("line: %s*****\n", line.c_str());
	    index++; // inc past newline
	    lastLineIndex = index;
	    newline = true;
	    break;
	}	

	index++;

	if( index == charsRead ) {
	    line.append(&buffer[lastLineIndex], index - lastLineIndex); // carry over
	    
	    if( !readMore() ) {
		// handle end of file case
		remainingLine += line;
		eof = true;
		break;
	    } 
	}
    }

    if( newline ) {
	if( index == charsRead ) {
	    readMore(); // even if readMore fails, we have to ret true here to proc newline
	}
	return true;  // goldilocks
    }

    // if we're at the end of the buffer here but eof != true,
    // then we were goldilocks and had to return true last call to getline
    if( index == charsRead || eof ) {
	return false;
    }
    
    return true;
}

void FileObject::closeFile() {
    fclose(theFile);
    theFile = nullptr;
    fileDescriptor = 0;
}
