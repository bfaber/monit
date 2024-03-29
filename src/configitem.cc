
#include "configitem.h"

ConfigItem::ConfigItem() {}

ConfigItem::ConfigItem(ConfigItem&& from) : name(std::move(from.name)),
					    regex(std::move(from.regex)),
					    csv(std::move(from.csv)),
					    collectionName(std::move(from.collectionName)),
					    httpEndpoint(std::move(from.httpEndpoint)),
					    filename(std::move(from.filename)) {
    pcreCompiledRegex = from.pcreCompiledRegex;
    jitStack = from.jitStack;
    extra    = from.extra;
}
					    
// this is getting instantiated from const char*s provided by the bson mongo query,
// so those initial values will be destroyed after this is created.
// string allocates these char* on the heap for us, but we'll check that they're not
// null before constructing here. 
	
void ConfigItem::setName(const char* n) {
    name = std::string(n);
}
void ConfigItem::setCsv(const char* c) {
    csv = std::string(c);
}
void ConfigItem::setRegex(const char* r) {
    regex = std::string(r);
}
void ConfigItem::setCompiledRegex(const pcre* compiledRegex) {
    pcreCompiledRegex = compiledRegex;
}
void ConfigItem::setJitStack(pcre_jit_stack* jit) {
    jitStack = jit;
}
void ConfigItem::setJitExtra(pcre_extra* e) {
    extra = e;
}
void ConfigItem::setCollectionName(const char* collName) {
    collectionName = std::string(collName);
}
void ConfigItem::setHttpEndpoint(const char* httpEndpt) {
    httpEndpoint = std::string(httpEndpt);
}
void ConfigItem::setFileName(const char* filen) {
    filename = std::string(filen);
}

std::string ConfigItem::getName() {
    return name;
}
std::string ConfigItem::getRegex() {
    return regex;
}
const pcre* ConfigItem::getCompiledRegex() {
    return pcreCompiledRegex;
}
pcre_jit_stack* ConfigItem::getJitStack() {
    return jitStack;
}
pcre_extra* ConfigItem::getJitExtra() {
    return extra;
}
std::string ConfigItem::getCollectionName() {
    return collectionName;
}
std::string ConfigItem::getHttpEndpoint() {
    return httpEndpoint;
}
std::string ConfigItem::getCsv() {
    return csv;
}
std::string ConfigItem::getFileName() {
    return filename;
}

// check to see if values set by mongo are here.
bool ConfigItem::isValid() {
    if(name.size() == 0)
	return false;
    else if(csv.size() == 0)
	return false;
    else if(regex.size() == 0)
	return false;
    else if(collectionName.size() == 0)
	return false;
    else if(filename.size() == 0)
	return false;
    else
	return true;
}
