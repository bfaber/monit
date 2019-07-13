#include "logreader.h"
#include "testprocessor.h"
#include "util.h"
#include "gtest/gtest.h"

#include <unistd.h>
#include <fstream>
//#include <filesystem>

class LogReaderTest : public ::testing::Test {
public:
    std::string file;
    std::string regex;
    LogReader *reader;
    TestProcessor *processor;
    ConfigItem *ci;

    // this is run per test below.  No way around this.
    // the entire logreadertest fixture is created and dest each test.

    // also this is the only place we are in the LogReader scope/ friend range.
    // So no ability to call private functions on LogReader from individual tests.
    // This is super weak requires running basically test in SetUp and checking results
    // in the test_f functions.
    void SetUp() override {
	// instantiate LogReader, use test file
	// test file hardcoded for now,
	// use test config data in db
	printf("set up!\n");
	file = "/Users/bfaber/Desktop/persephony/50kATC401.dat";
	regex = "^.*requestId\\\"\:\\\"(RQ[a-z0-9]{40}).*$";
	processor = new TestProcessor("requestId");
	ci = new ConfigItem();
	ci->setName("testLogReader");
	ci->setRegex(regex.c_str());
	ci->setFileName(file.c_str());
	ci->setCsv("requestId");
	ci->setCollectionName("testLogReader");
	std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
	configs->push_back(ci);
	reader = new LogReader(configs, processor);
	reader->readFile();
    };
    
    void TearDown() override  {
	
    };    
};

	

TEST_F(LogReaderTest, LogReaderGetsRequestIds) {
    // TODO: use c++17 filesystem for nicer things
    //    EXPECT_TRUE(std::filesystem::exists(file));
    std::ifstream f(file.c_str());
    EXPECT_TRUE(f.good());

    std::vector<std::string> groups;
    // matchstr is a single line from the log file referenced in setup
    std::string matchStr = "06/30 02:22:07.287 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"502\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\",\"vcsCallId\":\"CAd33388ab50758932e0f1b0111b9f5589127f44f8\",\"vcsScriptId\":\"58b1200536d63c4aac4df9b4ebaa00c0be030b94\",\"interruptIndex\":0,\"event\":\"final\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":{}},\"request_number\":\"402167\",\"location\":\"172.25.104.4:5237\"}}";
    reader->findGroups(matchStr, reader->compileRegex(regex), groups);
    EXPECT_EQ(1, groups.size());

    EXPECT_EQ(50787, processor->getMatchBufferSize());
}

TEST_F(LogReaderTest, UtilStringSplitTest)  {
    std::string csv = "requestId,accountId";
    std::vector<std::string> csvs;
    csvs = Util::splitString(csv, ',');
    EXPECT_EQ(2, csvs.size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int res = RUN_ALL_TESTS();
    return res;
}
