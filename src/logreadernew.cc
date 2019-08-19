#include <stdlib.h>
#include <cstdio>

#include "logreadernew.h"
#include "matchbundle.h"
#include "util.h"
#include "filereader.h"
#include "textparser.h"


LogReaderNew::LogReaderNew(std::vector<ConfigItem*> *cfgs, RecordProcessorInterface *recprocessor) :
    processor(recprocessor) {
    
    // TODO: need to parse errors if exist, and probably wrap the construction of
    // this reader so that compilation failure can be cleanly handled.
    for(auto *configitem : *cfgs) {
	printf("Compiling regex: %s\n", configitem->getRegex().c_str());
	const pcre* compiled_regex = TextParser::compileRegex(configitem->getRegex());
	const char* error;
	pcre_jit_stack *jit_stack = pcre_jit_stack_alloc(32*1024, 512*1024);	
	pcre_extra *extra = pcre_study(compiled_regex, PCRE_STUDY_JIT_COMPILE, &error);
	// Check for error (NULL)
	if(extra == nullptr) {
	    printf("ERROR regex study: %s\n", error);
	}
	pcre_assign_jit_stack(extra, NULL, jit_stack);
	configitem->setCompiledRegex(compiled_regex);
	configitem->setJitStack(jit_stack);
	configitem->setJitExtra(extra);
    }

    // set up configs per log file to make parsing more efficient
    for(auto *config : *cfgs) {
	auto *mb = new MatchBundle(config);
	matchBundlesPerFilename[config->getFileName()].push_back(mb);
    }    
}


/**
 * Read the file line by line, parse each line with regex, capturing whatever groups exist.
 * Group count given by pcre_exec return value (rc).  
 * readFile will use the interface provided by the RecordProcessorInterface for dealing with matches.  The matches vector will be allocated here and then moved to the RecordProcessor via uniquepointer.
 */
bool LogReaderNew::readFiles() {
    // map<filename, vec<matchbundle>>
    for( auto &kv : matchBundlesPerFilename ) {

	// the logreader process should be able to run without existence of
	// log files.  Those processes might be yet to be started.
	// todo: we might want to complain if log dne (configurable)
	if( Util::timeMs() < kv.second[0]->getRetryTime() ) {	
	    continue;
	}
	
	std::string filename = kv.first;
	std::vector<MatchBundle*> bundles = kv.second;
	    
	const char* logfilename = filename.c_str();
	//	printf("logfilename: %s\n", logfilename);

	std::ifstream logfile;
	logfile.open(logfilename, std::ios_base::in);
	
	if( logfile.is_open() ) {
	    //  printf("logfile open\n");
	    long t0 = Util::timeMs();
	    std::streamsize buffSize = 1024;
	    char buffer[buffSize];

	    int linect = 0;
	    //logfile.seekg(0, std::ios_base::end);
	    //std::size_t size = logfile.tellg();
	    //printf("File Size: %d\n", size);
	    //logfile.seekg(0, std::ios_base::beg);	    
	    //printf("file pos before: %ld\n", logfile.tellg());
	    //std::streambuf *buf = logfile.rdbuf();
	    //printf("streambuffer eback: %d, gptr: %d, egptr: %d\n", buf->eback(), buf->gptr(), buf->egptr());
	    //printf("inavail: %d\n", buf->in_avail());
	    while( logfile.getline(buffer, buffSize) ) {
		linect++;
		//printf("inavail: %d\n", buf->in_avail());
		//		printf("streambuffer eback: %d, gptr: %d, egptr: %d\n", buf->eback(), buf->gptr(), buf->egptr());
		//printf("file pos after: %ld\n", logfile.tellg());
	  
		/*

	    std::vector<char> wholeFile(size / sizeof(char));
	    logfile.read((char*) &wholeFile[0], size);
	    std::vector<std::string> lines;
	    parseFile(wholeFile, lines);
	    for(auto line : lines) {
		*/
	
	//auto *fileReader = new FileReader(kv.first);
	//std::vector<std::string> lines;
	//fileReader->getByLine(lines);
	//while(fileReader->getNextNLines2(lines, 1000)) {
	//}
		//	for(auto line : lines) {
		for( auto *mb : kv.second ) {
		    /*
		     * captures is a strange usage by pcre, must be size multiple of 3, 
		     * and only first 2 thirds will pass back captured substrings, 
		     * the rest is a 'workspace' by pcre_exec. See docs on 'ovector':
		     * https://pcre.org/original/doc/html/pcreapi.html
		     * captures contains set of pairs marking the beginning and end of 
		     * matching subgroups.
		     */
		    std::vector<std::string> groups;
		    const pcre* compiledRegex = mb->getConfigItem()->getCompiledRegex();
		    pcre_jit_stack *jit = mb->getConfigItem()->getJitStack();
		    pcre_extra *extra = mb->getConfigItem()->getJitExtra();
		    int rc = TextParser::findGroups(buffer, compiledRegex, jit, extra, groups);
		    if( ! groups.empty() ) {
			//printf("csv: %s\n", csv.c_str());
			mb->addGroups(groups);
		    } else {		    
			//printf("some other negative value for rc %d\n", rc);
		    }
		}

	    }

	    long t1 = Util::timeMs();
	    printf("logfileRead %s:DT: %ldms\n", logfilename, (t1 - t0));
	    // these should be all for the same collection?
	    // no, for the same file, not necessarily the same collection
	    // its processor responsibility to sort by collection to build records
	    processor->receiveMatches(kv.second);
	} else {
	    // mark this logfile as not existing atm, should attempt to read again
	    // in a little bit, do other work instead.
	    kv.second[0]->retry(Util::timeMs() + 500);
	}	    
    }        
    return true;   
}


