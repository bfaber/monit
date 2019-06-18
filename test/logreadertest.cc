#include "logreader.h"
#include "testprocessor.h"
#include "gtest/gtest.h"

class LogReaderTest : public ::testing::Test {
public:

    LogReader *reader;
    TestProcessor *processor;
    
    void SetUp() override {
	// instantiate LogReader, use test file
	// test file hardcoded for now,
	// use test config data in db
	printf("set up!\n");
	processor = new TestProcessor("requestId");
	reader = new LogReader("/Users/bfaber/Desktop/persephony/50kATC401.dat", "^.*requestId\\\"\:\\\"(RQ[a-z0-9]{40}).*$", processor);


    };
    
    void TearDown() override  {

    };    
};

TEST_F(LogReaderTest, LogReaderGetsRequestIds) {

    reader->testReadFile();
    auto matches = processor->getMatches();
    EXPECT_EQ(50787, matches->size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int res = RUN_ALL_TESTS();
    return res;
}
