#include "logreadernew.h"
#include "matchbundle.h"

#include "util.h"
#include "mongointerface.h"
#include "mongospooler.h"
#include "testspooler.h"
#include "testprocessor.h"
#include "mongorecordprocessor.h"
#include "gtest/gtest.h"

#include <unistd.h>
#include <fstream>
//#include <filesystem>

class LogReaderNewTest : public ::testing::Test {
public:
    std::string file;
    std::string regex;
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
	    ci->setCollectionName("testLogReader");
	


    };
    
    void TearDown() override  {
	
    };    
};

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
    EXPECT_EQ(50787, captures.size());
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

    //    EXPECT_EQ(1, processor->getMatchBufferSize());
    
    processor->processMatches();
    ASSERT_EQ(1, testspool->recordQueueTest.size());

    // this should produce some results in mongo.
    // probably don't want to test that here.
    // would be nice to break this out a little more
    // and verify the records produced. 
    //    csv->processMatches();
    // using testspooler to get to records right before mongo.
    Record* rec = testspool->popRecord();
    ASSERT_NE(nullptr, rec);
    const bson_t *doc = rec->getDocs()[0];
    const char* json = bson_as_json(doc, nullptr);

    printf("testspooler record 0: %s\n", json);
    EXPECT_STREQ("{ \"requestId\" : \"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\" }", json);
}


TEST_F(LogReaderNewTest, UtilStringSplitTest)  {
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
