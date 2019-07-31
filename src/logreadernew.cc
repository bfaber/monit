#include <stdlib.h>
#include <cstdio>

#include "logreadernew.h"
#include "matchbundle.h"
#include "util.h"


LogReaderNew::LogReaderNew(std::vector<ConfigItem*> *cfgs, RecordProcessorInterface *recprocessor) :
    processor(recprocessor) {
    
    // TODO: need to parse errors if exist, and probably wrap the construction of
    // this reader so that compilation failure can be cleanly handled.
    for(auto *configitem : *cfgs) {
	printf("Compiling regex: %s\n", configitem->getRegex().c_str());
	const pcre* compiled_regex = compileRegex(configitem->getRegex());
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

const pcre* LogReaderNew::compileRegex(std::string regex) {
	const char* error;
	int error_offset;

	const pcre* compiled_regex = pcre_compile(regex.c_str(), 0, &error, &error_offset, NULL);

	if(error_offset != 0) {
	    printf("ERROR compiling REGEX: %s\n", error);
	    // throw exception
	}

	return compiled_regex;
}

int LogReaderNew::findGroups(const char* text, const pcre* compiledRegex, pcre_jit_stack* jit, pcre_extra *extra, std::vector<std::string> &groups) {
    
    int captures[30]; // docs call this 'ovector'
    int rc = pcre_jit_exec(compiledRegex, extra, text, strlen(text), 0, 0, captures, 30, jit);
    //    std::cout << text << '\n';
    if( rc == PCRE_ERROR_NOMATCH ) {
	//	std::cout << "no match!" << std::endl;
    } else if( rc > 0 ) {

	//	printf("MATCH! RC Value: %d\n", rc);
	std::string csv;
	for(int i = 0; i < rc-1; i++) {
	    // indicies are
	    int ii = 2*i+2;
	    int iii = 2*i+3;
	    int len = captures[iii] - captures[ii];
	    //printf("indicies %d, %d\n", ii, iii);
	    //printf("Subject Index and len: %d, %d\n", captures[ii], len);

	    //	    std::string substr = text.substr(captures[ii], len);
	    char subbuf[len];
	    memcpy( subbuf, &text[captures[ii]], len );
	    subbuf[len] = '\0';
	    groups.push_back(std::string(subbuf));
	}
    }
    return rc;
}

/**
 * Read the file line by line, parse each line with regex, capturing whatever groups exist.
 * Group count given by pcre_exec return value (rc).  
 * readFile will use the interface provided by the RecordProcessorInterface for dealing with matches.  The matches vector will be allocated here and then moved to the RecordProcessor via uniquepointer.*  TODO:  configItems should be bundled by filename so that all regexes assoc with a file
 *  can be run when the file is open.  There should not be opening and reading of the file
 *  multiple times.   
 */
bool LogReaderNew::readFiles() {
    printf("reading files!\n");
    // map<filename, vec<matchbundle>>
    for(auto &kv : matchBundlesPerFilename) {
	std::ifstream logfile;
	const char* logfilename = kv.first.c_str();
	//	printf("logfilename: %s\n", logfilename);
	long t0 = Util::timeMs();
	logfile.open(logfilename, std::ios_base::in);
	if( logfile.is_open() ) {
	    //  printf("logfile open\n");
	    
	    char buffer[1024];
	    std::streamsize buffSize = 1024;
	    int linect = 0;
	    while(logfile.getline(buffer, buffSize)) {
	    
		/*
	    logfile.seekg(0, std::ios_base::end);
	    std::size_t size = logfile.tellg();
	    printf("File Size: %d\n", size);
	    logfile.seekg(0, std::ios_base::beg);
	    std::vector<char> wholeFile(size / sizeof(char));
	    logfile.read((char*) &wholeFile[0], size);
	    std::vector<std::string> lines;
	    parseFile(wholeFile, lines);
	    for(auto line : lines) {
		*/
		for(auto *mb : kv.second) {
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
		    int rc = findGroups(buffer, compiledRegex, jit, extra, groups);
		    if( ! groups.empty() ) {
			//printf("csv: %s\n", csv.c_str());
			mb->addGroups(groups);
		    } else {		    
			//printf("some other negative value for rc %d\n", rc);
		    }
		}
	    }

	    long t1 = Util::timeMs();
	    printf("logfileRead DT: %ldms\n", (t1 - t0));
	    // these should be all for the same collection?
	    // no, for the same file, not necessarily the same collection
	    // its processor responsibility to sort by collection to build records
	    processor->receiveMatches(kv.second);
	}
    }        
    return true;   
}


