#include "gtest/gtest.h"

#include "filereader.h"
#include "util.h"

class FileReaderTest : public ::testing::Test {    
public:

    FileReader *fileReader;
    
    void SetUp() override {
	std::string filename = "/Users/bfaber/Desktop/persephony/50kATC401.dat";
	fileReader = new FileReader(filename);

    };
    void TearDown() override {
	fileReader->close();
    };

};

TEST_F(FileReaderTest, ReadFile) {

    std::vector<std::string> lines;

    fileReader->getNextNLines2(lines, 10);
    //    delete fileReader;
    ASSERT_EQ(10, lines.size());

    char* line0 = "06/30 02:22:07.287 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"502\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\",\"vcsCallId\":\"CAd33388ab50758932e0f1b0111b9f5589127f44f8\",\"vcsScriptId\":\"58b1200536d63c4aac4df9b4ebaa00c0be030b94\",\"interruptIndex\":0,\"event\":\"final\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":{}},\"request_number\":\"402167\",\"location\":\"172.25.104.4:5237\"}}\n";
    ASSERT_STREQ(line0, lines[0].c_str());

    char* line1 = "06/30 02:22:07.288 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"502\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ504b00d1e0ab3ef20f66e5c3c37ed35c479c4dd7\",\"vcsCallId\":\"CAee218eebe4e4cdd0c44c7a251b1323dda5467447\",\"vcsScriptId\":\"65fba509bd597a9de4b524a193964b9ff7ad55c5\",\"interruptIndex\":0,\"event\":\"final\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":{}},\"request_number\":\"402168\",\"location\":\"172.25.104.4:5237\"}}\n";
    ASSERT_STREQ(line1, lines[1].c_str());
    
    char *line10 = "06/30 02:22:09.195 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"502\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ464112adf2e5acf6e1face3fbbd73c790066d676\",\"vcsCallId\":\"CAf5d936997348885131fd6285716d944ed0bc7ccb\",\"vcsScriptId\":\"4656eefe7c3eb23bfa753267a3548ee1628e472f\",\"interruptIndex\":0,\"event\":\"final\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":{}},\"request_number\":\"402532\",\"location\":\"172.25.104.4:5237\"}}\n";
    ASSERT_STREQ(line10, lines[9].c_str());
    
    lines.clear();

    fileReader->getNextNLines2(lines, 10);
    ASSERT_EQ(10, lines.size());

    char *line11 = "06/30 02:22:09.327 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"502\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ70230cf70b547a3d6a205e4e29bc5ba6ab45ab58\",\"vcsCallId\":\"CA4c60c07c51311648ea7c8e6216acb17b4f4b9177\",\"vcsScriptId\":\"93b69344742bde16e45a430d9dd37027bb203eb9\",\"interruptIndex\":0,\"event\":\"final\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":{}},\"request_number\":\"402564\",\"location\":\"172.25.104.4:5237\"}}\n";    
    ASSERT_STREQ(line11, lines[0].c_str());

    char *line20 = "06/30 02:22:10.320 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"512\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQc7d14911593e0be6c717c92e39904978add5f049\",\"vcsCallId\":\"CA0f3fcf44e705946ea163b39a970e5034f264122b\",\"vcsScriptId\":\"681618ebefaffa7f27e9f013341a5f1231d2b596\",\"interruptIndex\":3,\"event\":\"addToConference\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":\"\"},\"request_number\":\"389069\",\"location\":\"172.25.104.4:5237\"}}\n";
    ASSERT_STREQ(line20, lines[9].c_str());
}

TEST_F(FileReaderTest, ReadWholeFileDoNothing) {
    long t0 = Util::timeMs();
    fileReader->readWholeFile();
    long t1 = Util::timeMs();
    printf("ReadWholeFileDoNothing DT: %ldms\n", (t1 - t0));    
}

TEST_F(FileReaderTest, ReadWholeFileByOneThousand) {
    // there are 50787 lines in the log file.
    // get them all, see if last line matches.

    int loopCt = 0;
    std::vector<std::string> lines;
    long t0 = Util::timeMs();
    lines.reserve(1000);
    while(fileReader->getNextNLines2(lines, 1000)) {
	if(lines.size() != 1000) {
	    EXPECT_EQ(787, lines.size());
	} else {
	    EXPECT_EQ(1000, lines.size());
            lines.clear();
	}

	loopCt++;
    }
    long t1 = Util::timeMs();
    printf("ReadWholeFileByOneThousand::DT %ldms\n", (t1 - t0));
    EXPECT_EQ(51, loopCt);
    EXPECT_EQ(787, lines.size());
    
    char *lastLine = "06/30 04:20:38.891 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"513\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ353db8eb0eeba60cab5b5da042112dd6c0dd50e4\",\"vcsCallId\":\"CA18457d49dce8562dd3ff537429bfd5153b654d09\",\"vcsScriptId\":\"2bbf3fa6e1a7802994a2e376a62d42a4945ee508\",\"interruptIndex\":3,\"event\":\"addToConference\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":\"\"},\"request_number\":\"1681567\",\"location\":\"172.25.104.4:5237\"}}\n";
    EXPECT_STREQ(lastLine, lines[lines.size() - 1].c_str());
}

TEST_F(FileReaderTest, ReadWholeFileByOneHundred) {
    long t0 = Util::timeMs();
    int lineCt = 0;
    std::vector<std::string> lines;
    while(fileReader->getNextNLines2(lines, 100)) {
	if(lines.size() != 100) {
	    EXPECT_EQ(87, lines.size());
	    lineCt += 87;
	} else {
	    EXPECT_EQ(100, lines.size());
	    lineCt += 100;
	}
	lines.clear();
    }
    long t1 = Util::timeMs();
    printf("ReadWholeFileByOneHundred::DT %ldms\n", (t1 - t0));
    EXPECT_EQ(50787, lineCt);    
}

TEST_F(FileReaderTest, ReadWholeFileByTen) {
    long t0 = Util::timeMs();
    int lineCt = 0;
    std::vector<std::string> lines;
    while(fileReader->getNextNLines2(lines, 10)) {
	if(lines.size() != 10) {
	    EXPECT_EQ(7, lines.size());
	    lineCt += 7;
	} else {
	    EXPECT_EQ(10, lines.size());
	    lineCt += 10;
	}
	lines.clear();
    }
    long t1 = Util::timeMs();
    printf("ReadWholeFileByOneHundred::DT %ldms\n", (t1 - t0));
    EXPECT_EQ(50787, lineCt);    
}
TEST_F(FileReaderTest, ReadWholeFileByFour) {
    long t0 = Util::timeMs();
    int lineCt = 0;
    std::vector<std::string> lines;
    while(fileReader->getNextNLines2(lines, 4)) {
	if(lines.size() != 4) {
	    EXPECT_EQ(3, lines.size());
	    lineCt += 3;
	} else {
	    EXPECT_EQ(4, lines.size());
	    lineCt += 4;
	}
	lines.clear();
    }
    long t1 = Util::timeMs();
    printf("ReadWholeFileByOneFour::DT %ldms\n", (t1 - t0));
    EXPECT_EQ(50787, lineCt);    
}

TEST_F(FileReaderTest, ReadWholeFileByTenThousand) {
    long t0 = Util::timeMs();
    int lineCt = 0;
    std::vector<std::string> lines;
    while(fileReader->getNextNLines2(lines, 10000)) {
	if(lines.size() != 10000) {
	    EXPECT_EQ(10787, lines.size());
	    lineCt += 10787;
	} else {
	    EXPECT_EQ(10000, lines.size());
	    lineCt += 10000;
	}
	lines.clear();
    }
    long t1 = Util::timeMs();
    printf("ReadWholeFileByOneHundred::DT %ldms\n", (t1 - t0));
    EXPECT_EQ(50787, lineCt);    
}

