#include "textparser.h"

const pcre* TextParser::compileRegex(std::string regex) {
	const char* error;
	int error_offset;

	const pcre* compiled_regex = pcre_compile(regex.c_str(), 0, &error, &error_offset, NULL);

	if(error_offset != 0) {
	    printf("ERROR compiling REGEX: %s\n", error);
	    // throw exception
	}

	return compiled_regex;
}

int TextParser::findGroups(const char* text, const pcre* compiledRegex, pcre_jit_stack* jit, pcre_extra *extra, std::vector<std::string> &groups) {
    
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
