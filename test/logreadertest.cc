#include "logreader.h"
#include "gtest/gtest.h"

class LogReaderTest : public ::testing::Test {
public:
    LogReader *reader;
    CSVProcessor *processor;
    std::thread readerThread;
    std::thread processorThread;
    
    void SetUp() override {
	// instantiate LogReader, use test file
	// test file hardcoded for now,
	// use test config data in db

	processor = new CSVProcessor("requestId");
	reader = new LogReader("/Users/bfaber/Desktop/persephony/50kATC401.dat", "^.*requestId\\\"\:\\\"(RQ[a-z0-9]{40}).*$", processor);

	readerThread = LogReader::start(reader);
	//	processorThread = CSVProcessor::start(processor);

    };
    
    void TearDown() override  {
	reader->shutdownThread();
	processor->shutdownThread();

	readerThread.join();
	//	processorThread.join();

    };    
};

TEST_F(LogReaderTest, LogReaderGetsRequestIds) {

    auto matches = processor->getMatches();
    ASSERT_EQ(50787, matches->size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int res = RUN_ALL_TESTS();
    /*    if(res == 0)
	printf("tests pass\n");
    else
	printf("test fails\n");
    */
    return res;
}
