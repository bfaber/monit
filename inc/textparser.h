#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <string>
#include <vector>

#include "pcre.h"

class TextParser {
 public:
    static int  findGroups(const char* text, const pcre *comiledRegex, pcre_jit_stack* jit, pcre_extra *extra, std::vector<std::string> &groups);
    static const pcre* compileRegex(std::string regex);

};
#endif
