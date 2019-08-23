#include "logreadernew.h"
#include "matchbundle.h"

#include "util.h"
#include "mongointerface.h"
#include "mongospooler.h"
#include "testspooler.h"
#include "testprocessor.h"
#include "mongorecordprocessor.h"
#include "textparser.h"
#include "gtest/gtest.h"

#include <unistd.h>
#include <fstream>
//#include <filesystem>

class LogReaderNewTest : public ::testing::Test {
public:
    std::string file;
    std::string regex;
    std::string regex2;
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
	    regex  = "^.*requestId\\\"\:\\\"(RQ[a-z0-9]{40}).*$";
	    regex2 = "^.*requestId\"\: \"([0-9a-z-]{36})\"\}$";

	    /*
	      std::string mongohost = std::string("localhost");
	      int mongoport = 27017;
	      std::string dbName = std::string("test");

	      auto *mi = new MongoInterface(mongohost, mongoport, dbName);
	      auto *ms = new MongoSpooler(mi);
	    */
	

	    ci = new ConfigItem();
	    ci->setName("testLogReader");
	    ci->setRegex(regex.c_str());
	    ci->setFileName(file.c_str());
	    ci->setCsv("requestId");
	    ci->setCollectionName("testResultData");
	


    };
    
    void TearDown() override  {
	
    };    
};

TEST_F(LogReaderNewTest, CheckRegexParsingFileOne) {
    // see filereadertest for this same line
    char* line0 = "06/30 02:22:07.287 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"502\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\",\"vcsCallId\":\"CAd33388ab50758932e0f1b0111b9f5589127f44f8\",\"vcsScriptId\":\"58b1200536d63c4aac4df9b4ebaa00c0be030b94\",\"interruptIndex\":0,\"event\":\"final\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":{}},\"request_number\":\"402167\",\"location\":\"172.25.104.4:5237\"}}\n";

    const char* error;
    const pcre* compiledRegex = TextParser::compileRegex(regex);
    pcre_jit_stack *jit_stack = pcre_jit_stack_alloc(32*1024, 512*1024);	
    pcre_extra *extra = pcre_study(compiledRegex, PCRE_STUDY_JIT_COMPILE, &error);
    pcre_assign_jit_stack(extra, nullptr, jit_stack);

    std::vector<std::string> groups;
    TextParser::findGroups(line0, compiledRegex, jit_stack, extra, groups);

    EXPECT_EQ(1, groups.size());

}

TEST_F(LogReaderNewTest, CheckRegexParsingFileTwo) {
    // see filereadertest for this same line
    char* line1 = "logging stmt: {\"machineId\": \"4azzz58ib\", \"userId\": \"39d3a40a-bfcb-11e9-b05d-34363bd0b9a8\", \"requestId\": \"39d3a40a-bfcb-11e9-b05d-34363bd0b9a8\"}\n";



    const char* error;
    const pcre* compiledRegex = TextParser::compileRegex(regex2);
    pcre_jit_stack *jit_stack = pcre_jit_stack_alloc(32*1024, 512*1024);	
    pcre_extra *extra = pcre_study(compiledRegex, PCRE_STUDY_JIT_COMPILE, &error);
    pcre_assign_jit_stack(extra, nullptr, jit_stack);

    printf("line1: %s\n", line1);
    std::vector<std::string> groups;
    TextParser::findGroups(line1, compiledRegex, jit_stack, extra, groups);

    ASSERT_EQ(1, groups.size());
    printf("group caught: %s\n", groups[0].c_str());
}

TEST_F(LogReaderNewTest, CheckProcessorReceivesMatches) {
    auto *testspool = new TestSpooler();
    auto *testproc = new TestProcessor(testspool);
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    configs->push_back(ci);

    auto *reader = new LogReaderNew(configs, testproc);
    
    reader->readFiles();

    std::vector<MatchBundle*> bundles;
    testproc->getMatches(bundles);
    ASSERT_EQ(1, bundles.size());

    std::vector<std::vector<std::string>> captures = bundles[0]->getBundle();
    ASSERT_EQ(50787, captures.size());
}

TEST_F(LogReaderNewTest, CheckProcessorReceivesMatchesTwoFiles) {
    auto *testspool = new TestSpooler();
    auto *testproc = new TestProcessor(testspool);
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    configs->push_back(ci);
    auto *ci2 = new ConfigItem();
    ci2->setName("movingLog");
    ci2->setRegex(regex2.c_str());
    std::string file2 = "/Users/bfaber/Desktop/play/cpp/monit/test/test2files.log";
    ci2->setFileName(file2.c_str());
    ci2->setCsv("requestId");
    ci2->setCollectionName("testMovingLogData");
    configs->push_back(ci2);

    auto *reader = new LogReaderNew(configs, testproc);

    
    reader->readFiles();

    std::vector<MatchBundle*> bundles;
    testproc->getMatches(bundles);
    ASSERT_EQ(2, bundles.size());

    std::vector<std::vector<std::string>> captures = bundles[0]->getBundle();
    ASSERT_EQ(50787, captures.size());
    std::vector<std::vector<std::string>> captures2 = bundles[1]->getBundle();
    ASSERT_EQ(525, captures2.size());    
}

TEST_F(LogReaderNewTest, CheckProcessorProcessesMatches) {
    // TODO: use c++17 filesystem for nicer things
    //    EXPECT_TRUE(std::filesystem::exists(file));
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    configs->push_back(ci);
    //    auto *testmongo = new TestMongoInterface();
    auto *testspool = new TestSpooler();
    auto *processor = new MongoRecordProcessor(testspool);
    auto *reader = new LogReaderNew(configs, processor);

    std::ifstream f(file.c_str());
    ASSERT_TRUE(f.good());        

    reader->readFiles();

    // after readfiles completes, receiveMatches was called, copying
    // everything out of the match bundle buffers
    EXPECT_EQ(0, processor->getMatchBufferSize());
    
    processor->processMatches();
    ASSERT_EQ(1, testspool->recordQueueTest.size());

    // this should produce some results in mongo.
    // probably don't want to test that here.
    // would be nice to break this out a little more
    // and verify the records produced. 
    //    csv->processMatches();
    // using testspooler to get to records right before mongo.
    Record* rec = testspool->recordQueueTest[0];
    ASSERT_NE(nullptr, rec);
    ASSERT_NE(rec->getDocs(), nullptr);
    const bson_t *doc = rec->getDocs()[0];
    const char* json = bson_as_json(doc, nullptr);

    printf("testspooler record 0: %s\n", json);
    EXPECT_STREQ("{ \"requestId\" : \"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\" }", json);
}

TEST_F(LogReaderNewTest, CheckReaderMultipleCallsToReadFiles) {
    // TODO: use c++17 filesystem for nicer things
    //    EXPECT_TRUE(std::filesystem::exists(file));
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    configs->push_back(ci);
    //    auto *testmongo = new TestMongoInterface();
    auto *testspool = new TestSpooler();
    auto *processor = new MongoRecordProcessor(testspool);
    auto *reader = new LogReaderNew(configs, processor);

    std::ifstream f(file.c_str());
    ASSERT_TRUE(f.good());        

    reader->readFiles();

    EXPECT_EQ(0, processor->getMatchBufferSize());
    
    reader->readFiles(); // shouldnt get more matches.
    EXPECT_EQ(0, processor->getMatchBufferSize());
    processor->processMatches();
    ASSERT_EQ(1, testspool->size());

    
    Record* rec = testspool->recordQueueTest[0];    
    EXPECT_EQ(50787, rec->size());
}

TEST_F(LogReaderNewTest, CheckProcessorProcessesMatchesTwoFiles) {
    // TODO: use c++17 filesystem for nicer things
    //    EXPECT_TRUE(std::filesystem::exists(file));
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    configs->push_back(ci);
    //{ "_id" : ObjectId("5d544e526f8b67931c793e2d"), "regex" : "^.*requestId\\\"\\: \\\"([\\-a-f0-9]{37})\\\"\\}$", "matchItemName" : "movingLog", "filename" : "/Users/bfaber/Desktop/play/cpp/monit/test/testing.log", "csv" : "requestId", "collectionName" : "testMovingLogData" }
    auto *ci2 = new ConfigItem();
    ci2->setName("movingLog");
    //ci2->setRegex("^.*requestId\":\"([\-a-f0-9]{37})\"\}$");
    ci2->setRegex(regex2.c_str());
    std::string file2 = "/Users/bfaber/Desktop/play/cpp/monit/test/test2files.log";
    ci2->setFileName(file2.c_str());
    ci2->setCsv("requestId");
    ci2->setCollectionName("testMovingLogData");
    configs->push_back(ci2);

    //    auto *testmongo = new TestMongoInterface();
    auto *testspool = new TestSpooler();
    auto *processor = new MongoRecordProcessor(testspool);
    auto *reader = new LogReaderNew(configs, processor);

    std::ifstream f(file.c_str());
    ASSERT_TRUE(f.good());
    std::ifstream f2(file2.c_str());
    ASSERT_TRUE(f2.good());

    reader->readFiles();

    EXPECT_EQ(0, processor->getMatchBufferSize());
    
    processor->processMatches();
    ASSERT_EQ(2, testspool->recordQueueTest.size());

    // this should produce some results in mongo.
    // probably don't want to test that here.
    // would be nice to break this out a little more
    // and verify the records produced. 
    //    csv->processMatches();
    // using testspooler to get to records right before mongo.
    ASSERT_EQ(2, testspool->size());
    for( auto *rec : testspool->recordQueueTest ) {
	if( rec->getCollectionName() == std::string("testResultData")) {
	    ASSERT_EQ(rec->getCollectionName(), "testResultData");
	    ASSERT_EQ(rec->size(), 50787);
	    ASSERT_NE(nullptr, rec);
	    ASSERT_NE(rec->getDocs(), nullptr);
	    const bson_t *doc = rec->getDocs()[0];
	    const char* json = bson_as_json(doc, nullptr);

	    printf("testspooler record 0: %s\n", json);
	    EXPECT_STREQ("{ \"requestId\" : \"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\" }", json);
	} else {
	    ASSERT_EQ(rec->getCollectionName(), "testMovingLogData");
	    ASSERT_EQ(rec->size(), 525);
	    ASSERT_NE(nullptr, rec);
	    ASSERT_NE(rec->getDocs(), nullptr);
	    const bson_t *doc2 = rec->getDocs()[0];
	    const char* json2 = bson_as_json(doc2, nullptr);

	    printf("testspooler record 1: %s\n", json2);
	    EXPECT_STREQ("{ \"requestId\" : \"24265df5-bee4-11e9-9c11-34363bd0b9a8\" }", json2);
	}
    }
}

TEST_F(LogReaderNewTest, CheckProcessorProcessesMatchesMultipleLogFilesOneDNE) {
    // TODO: use c++17 filesystem for nicer things
    //    EXPECT_TRUE(std::filesystem::exists(file));
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    configs->push_back(ci);

    auto *ci2 = new ConfigItem();
    ci2->setName("testLogReader");
    ci2->setRegex(regex.c_str());
    ci2->setFileName("/Users/bfaber/Desktop/play/cpp/monit/fileDNE.log");
    ci2->setCsv("requestId");
    ci2->setCollectionName("testFileDNEData");

    configs->push_back(ci2);

    //    auto *testmongo = new TestMongoInterface();
    auto *testspool = new TestSpooler();
    auto *processor = new MongoRecordProcessor(testspool);
    auto *reader = new LogReaderNew(configs, processor);

    std::ifstream f(file.c_str());
    ASSERT_TRUE(f.good());        

    reader->readFiles();
    
    EXPECT_EQ(0, processor->getMatchBufferSize());
    
    processor->processMatches();
    ASSERT_EQ(1, testspool->recordQueueTest.size());

    // this should produce some results in mongo.
    // probably don't want to test that here.
    // would be nice to break this out a little more
    // and verify the records produced. 
    //    csv->processMatches();
    // using testspooler to get to records right before mongo.
    ASSERT_EQ(testspool->size(), 1);    
    Record* rec = testspool->recordQueueTest[0];
    ASSERT_EQ(rec->getCollectionName(), "testResultData");
    ASSERT_NE(nullptr, rec);
    ASSERT_NE(rec->getDocs(), nullptr);
    const bson_t *doc = rec->getDocs()[0];
    const char* json = bson_as_json(doc, nullptr);

    printf("testspooler record 0: %s\n", json);
    EXPECT_STREQ("{ \"requestId\" : \"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\" }", json);
}

TEST_F(LogReaderNewTest, CheckProcessesMatchesLogFileDNE) {
    // TODO: use c++17 filesystem for nicer things
    //    EXPECT_TRUE(std::filesystem::exists(file));
    std::vector<ConfigItem*> *configs = new std::vector<ConfigItem*>();
    
    auto *ci2 = new ConfigItem();
    ci2->setName("testLogReader");
    ci2->setRegex(regex.c_str());
    ci2->setFileName("/Users/bfaber/Desktop/play/cpp/monit/fileDNE.log");
    ci2->setCsv("requestId");
    ci2->setCollectionName("thisCollectionShouldntExist");

    configs->push_back(ci2);

    //    auto *testmongo = new TestMongoInterface();
    auto *testspool = new TestSpooler();
    auto *processor = new MongoRecordProcessor(testspool);
    auto *reader = new LogReaderNew(configs, processor);

    reader->readFiles();

    EXPECT_EQ(0, processor->getMatchBufferSize());
    
    processor->processMatches();
    ASSERT_EQ(0, testspool->recordQueueTest.size());

    // this should produce some results in mongo.
    // probably don't want to test that here.
    // would be nice to break this out a little more
    // and verify the records produced. 
    //    csv->processMatches();
    // using testspooler to get to records right before mongo.
    ASSERT_EQ(0, testspool->size());

}

TEST_F(LogReaderNewTest, UtilStringSplitTest)  {
    std::string csv = "requestId,accountId";
    std::vector<std::string> csvs;
    csvs = Util::splitString(csv, ',');
    EXPECT_EQ(2, csvs.size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "LogReaderNewTest.CheckRegexParsingFileTwo";
    //::testing::GTEST_FLAG(filter) = "LogReaderNewTest.CheckReaderMultipleCallsToReadFiles";
    int res = RUN_ALL_TESTS();
    return res;
}
