#include <thread>
#include <stdlib.h>

#include "logreader.h"

LogReader::LogReader(const char* filename, const char* regex, MatchProcessor *csvprocessor) :
    threadStarted(false),
    threadStopped(false),
    shutdown(false),
    logfilename(strdup(filename)),
    regex(regex)  {
    
    // TODO: need to parse errors if exist, and probably wrap the construction of
    // this reader so that compilation failure can be cleanly handled.
    const char* error;
    int error_offset;
    compiled_regex = pcre_compile(regex, 0, &error, &error_offset, NULL);
    if(error_offset != 0) {
	printf("ERROR compiling REGEX: %s\n", error);
    }

    processor = csvprocessor;
}

// have to specify both move and copy ctors
LogReader::LogReader(LogReader&& other) : threadStarted(other.threadStarted),
					  threadStopped(other.threadStopped),
					  logfilename(strdup(other.logfilename)),
					  regex(strdup(other.regex))  {
    printf("move constructor called on LogReader\n");
    compiled_regex= other.compiled_regex;
    
    processor     = other.processor;	
}

LogReader::LogReader(LogReader const &other) {
    printf("copy constructor called on LogReader\n");
    threadStarted = other.threadStarted;
    threadStopped = other.threadStopped;
    logfilename   = strdup(other.logfilename);
    regex         = strdup(other.regex);
    compiled_regex= other.compiled_regex;
    
    processor     = other.processor;	

}

LogReader::~LogReader() {
    if(logfile.is_open())
	logfile.close();
    printf("deleting log reader!\n");
    delete logfilename;
    logfilename = nullptr;
    delete regex;
    regex = nullptr;
}

std::thread LogReader::start(LogReader *inst) {    
    std::thread th(*inst);
    return th;
}

void LogReader::shutdownThread() {
    mutex.lock();
    shutdown = true;
    mutex.unlock();
}

void LogReader::operator()() {
    readFile();
    printf("hello world\n");
}

void LogReader::testReadFile() {
    readFile();
}
/**
 * Read the file line by line, parse each line with regex, capturing whatever groups exist.
 * Group count given by pcre_exec return value (rc).  
 * readFile will use the interface provided by the CSVProcessor for dealing with events.  The events vector will be allocated here and then moved to the CSVProcessor via uniquepointer.  
 */
bool LogReader::readFile() {
    printf("reading file!\n");
    std::string line;
    int linect = 0;
    // TODO: fix this, use move somehow
    std::vector<const char*> *matches = new std::vector<const char*>(); 

    printf("logfilename: %s\n", logfilename);
    logfile.open(logfilename, std::ios_base::in);
    
    if( logfile.is_open() ) {
	printf("logfile open\n");
	while( getline(logfile, line) ) {
	    linect++;
	    /*
	     * captures is a strange usage by pcre, must be size multiple of 3, 
	     * and only first 2 thirds will pass back captured substrings, 
	     * the rest is a 'workspace' by pcre_exec. See docs on 'ovector':
	     * https://pcre.org/original/doc/html/pcreapi.html
	     * captures contains set of pairs marking the beginning and end of 
	     * matching subgroups.
	     */
	    int captures[30]; // docs call this 'ovector'
	    int rc = pcre_exec(compiled_regex, NULL, line.c_str(), strlen(line.c_str()), 0, 0, captures, 30);

	    if( rc == PCRE_ERROR_NOMATCH ) {

	    } else if( rc > 0 ) {
		//std::cout << line << '\n';
		//printf("MATCH! RC Value: %d\n", rc);
		std::string csv;
		for(int i = 0; i < rc-1; i++) {
		    // indicies are
		    int ii = 2*i+2;
		    int iii = 2*i+3;
		    int len = captures[iii] - captures[ii];
		    //printf("indicies %d, %d\n", ii, iii);
		    //printf("Subject Index and len: %d, %d\n", captures[ii], len);
		    if( i == rc-2 )
			csv += line.substr(captures[ii], len);
		    else {
			csv += line.substr(captures[ii], len);
			csv += "|";
		    }		    
		}
		if( ! csv.empty() ) {
		    //printf("csv: %s\n", csv.c_str());
		    matches->push_back(strdup(csv.c_str()));
		}

	    } else {		    
		printf("some other negative value for rc %d\n", rc);
	    }


	}	
    }
    printf("lines searched: %d\n", linect);
    printf("matches collected: %d\n", matches->size());
    processor->receiveMatches(matches);
    return true;
}
