#ifndef CONFIGITEM_H
#define CONFIGITEM_H

#include <string>
#include "pcre.h"

/*
 * ConfigItem is a single item of config, ie a single regex.
 * That regex will have a number of things assoc with it. 
 * Filename is where that regex is to be run.
 * Http is the endpoint to be notified, etc. 
 * 
 */

class ConfigItem {
public:
    ConfigItem();

    bool isValid();
    
    void setName(const char* n);
    void setCsv(const char* n);
    void setRegex(const char* n);
    void setCompiledRegex(const pcre* n);
    void setCollectionName(const char* n);
    void setHttpEndpoint(const char* n);
    void setFileName(const char* n);

    std::string getName();
    std::string getRegex();
    const pcre* getCompiledRegex();
    std::string getCollectionName();
    std::string getHttpEndpoint();
    std::string getCsv();
    std::string getFileName();
    
private:
    std::string name;
    std::string regex;
    const pcre* pcreCompiledRegex;
    std::string csv;
    std::string collectionName;
    std::string httpEndpoint;
    std::string filename;
};
#endif
