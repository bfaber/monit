#include "gtest/gtest.h"

#include "fileobject.h"

class FileObjectTest : public ::testing::Test {
public:

    FileObject *fileObj;
    FileObject *growingFileObj;

    std::string filename;
    std::string growingFile;
    
    void SetUp() override {
	filename = "/Users/bfaber/Desktop/persephony/50kATC401.dat";
	growingFile = "/Users/bfaber/Desktop/play/cpp/monit/test/rotateTest.log";

	fileObj = new FileObject(filename);
	growingFileObj = new FileObject(growingFile);
    };
    void TearDown() override {};
};

TEST_F(FileObjectTest, ReadASingleLine) {
    ASSERT_TRUE(fileObj->openFile());
    std::string line;
    ASSERT_TRUE(fileObj->resume());
    ASSERT_TRUE(fileObj->getLine(line));

    char* line0 = "06/30 02:22:07.287 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"502\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ3279b11099d777e5ce4308aea6f1dc8d3bc72725\",\"vcsCallId\":\"CAd33388ab50758932e0f1b0111b9f5589127f44f8\",\"vcsScriptId\":\"58b1200536d63c4aac4df9b4ebaa00c0be030b94\",\"interruptIndex\":0,\"event\":\"final\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":{}},\"request_number\":\"402167\",\"location\":\"172.25.104.4:5237\"}}";

    ASSERT_STREQ(line0, line.c_str());
    line.clear();
    ASSERT_EQ(0, line.size());
    fileObj->closeFile();
}

TEST_F(FileObjectTest, ReadAllTheLines) {
    ASSERT_TRUE(fileObj->openFile());
    ASSERT_TRUE(fileObj->resume());
    std::string line;
    int lineCt = 0;
    while( fileObj->getLine(line) ) {	
	lineCt++;
	//	printf("lineCt: %d\n", lineCt);
	//	printf("line: %s\n", line.c_str());
	if( lineCt == 50787 ) {
	    char* lastLine = "06/30 04:20:38.891 {I} : {\"requestHeaders\" : {\"Host\" : \"127.0.0.1:8265\", \"Accept\" : \"*/*\", \"Content-Length\" : \"513\", \"Content-Type\" : \"application/x-www-form-urlencoded\"}, \"queryParameters\" : {}, \"requestBody\" : {\"accountId\":\"AC26f91427cd2fadb33869b09f8b0428e67f82be6f\",\"requestId\":\"RQ353db8eb0eeba60cab5b5da042112dd6c0dd50e4\",\"vcsCallId\":\"CA18457d49dce8562dd3ff537429bfd5153b654d09\",\"vcsScriptId\":\"2bbf3fa6e1a7802994a2e376a62d42a4945ee508\",\"interruptIndex\":3,\"event\":\"addToConference\",\"vclResult\":{\"lastCommand\":0,\"commandName\":\"AddToConference\",\"lastNestedCommand\":null,\"nestedCommandName\":null,\"diagnostics\":\"successful completion\",\"returncode\":0,\"resultData\":\"\"},\"request_number\":\"1681567\",\"location\":\"172.25.104.4:5237\"}}";
	    EXPECT_STREQ(lastLine, line.c_str());
	}

	
    }
    ASSERT_EQ(lineCt, 50787);
    fileObj->closeFile();
}

TEST_F(FileObjectTest, FileGrows) {
    ASSERT_TRUE(growingFileObj->openFile());
    ASSERT_TRUE(growingFileObj->resume());
    
    std::string line;
    int linect = 0;
    while( growingFileObj->getLine(line) ) {
	linect++;
	line.clear();
    }
    EXPECT_GT(linect, 100);
    line.clear();

    FILE *file = fopen(growingFile.c_str(), "a");
    // TODO:would be better if there was a random value generated here that would prove this is the last line retrieved and not already in the file
    const char* lastLine = "2019-08-23 14:27:20,602 DEBUG {\"machineId\": \"4azzz58ib\", \"userId\": \"ca2724eb-c5ec-11e9-800a-34363bd0b9a8\", \"requestId\": \"ca2724eb-c5ec-11e9-800a-34363bd0b9a8\"}\n";
    fwrite(lastLine, sizeof(char), strlen(lastLine), file);
    fclose(file);
    // doing the above, and then read() still returns no new chars read in the fileobj.
        
    // this is really interesting, the FILE stream must be per process?
    // seems dangerous but then again it is a C api.
    //growingFileObj->closeFile();
    //growingFileObj->openFile();
    EXPECT_TRUE(growingFileObj->resume());
    EXPECT_TRUE(growingFileObj->getLine(line));
    line += '\n';
    EXPECT_STREQ(line.c_str(), lastLine);
}

TEST_F(FileObjectTest, RotateFile) {
    ASSERT_TRUE(growingFileObj->openFile());
    ASSERT_TRUE(growingFileObj->resume());
    
    std::string line;
    int linect = 0;
    while( growingFileObj->getLine(line) ) {
	linect++;
	line.clear();
    }

    EXPECT_GT(linect, 100);
    line.clear();

    // now rotate file, 
    std::string newFileName = "/Users/bfaber/Desktop/play/cpp/monit/test/rotateTest1.log";
    int result= rename( growingFile.c_str(), newFileName.c_str() );
    ASSERT_EQ(result, 0);

    // this isnt a great test, doing this from the same process impacts the fd count
    // within the process, which wouldnt happen if the log producer was a sep proc.
    // This should be part of the integration test.
    FILE *newFile = fopen(growingFile.c_str(), "a");
    int fileA = fileno(newFile);
    const char* newLine = "2019-08-23 14:27:20,602 DEBUG {\"machineId\": \"4azzz58ib\", \"userId\": \"ca2724eb-c5ec-11e9-800a-34363bd0b9a8\", \"requestId\": \"ca2724eb-c5ec-11e9-777a-34363bd0b9a8\"}\n";
    fwrite(newLine, sizeof(char), strlen(newLine), newFile);
    fclose(newFile); // need to do this to flush this write, else resume() will fail.
    EXPECT_TRUE(growingFileObj->resume());
    EXPECT_TRUE(growingFileObj->getLine(line));
    line += '\n';
    EXPECT_STREQ(line.c_str(), newLine);
    fclose(newFile);

    ASSERT_EQ(rename(newFileName.c_str(), growingFile.c_str()), 0);
}

TEST_F(FileObjectTest, FileDescChange) {
    std::string fileName = "testFile.txt";
    FILE *fileA = fopen(fileName.c_str(), "a");
    int fileADesc = fileno(fileA);
    fwrite("hello\n", sizeof(char), strlen("hello"), fileA);
    //fclose(fileA);
    FILE *fileAStreamB = fopen(fileName.c_str(), "r");
    int fileAStreamBDesc = fileno(fileAStreamB);

    // each stream gets a uniq FD - if both are writing to the file - unexpected
    // else if one is "r", the FDs are the same - expected.
    // except the "r" case doesn't seem to be happening with our log file...
    EXPECT_NE(fileAStreamBDesc, fileADesc);
    
    std::string newName = "testFile1.txt";
    int res = rename( fileName.c_str(), newName.c_str());
    int renamedDesc = fileno(fileAStreamB);
    int fileARenDesc = fileno(fileA);
    // but at least each stream retains its FD after rename.
    EXPECT_EQ(fileAStreamBDesc, renamedDesc);
    EXPECT_EQ(fileARenDesc, fileADesc);

    // reopen a file to the original "testFile.txt"
    FILE *fileB = fopen(fileName.c_str(), "a");
    int newFileDesc = fileno(fileB);
    fwrite("hello again", sizeof(char), strlen("hello again"), fileB);
    EXPECT_NE(newFileDesc, fileADesc);	

    FILE *fileBAgain = fopen(fileName.c_str(), "r");
    int fileBAgainDesc = fileno(fileBAgain);
    EXPECT_NE(fileBAgainDesc, fileADesc);
    
    remove(fileName.c_str());
    remove(newName.c_str());
}

TEST_F(FileObjectTest, OpenReadOpenRead) {
    ASSERT_TRUE(fileObj->openFile());
    ASSERT_TRUE(fileObj->resume());

    std::string line;
    int lineCt = 0;
    while( fileObj->getLine(line) ) {
	lineCt++;
	line.clear();
    }
    EXPECT_EQ(50787, lineCt);

    ASSERT_TRUE(fileObj->openFile());
    ASSERT_FALSE(fileObj->resume());

    while( fileObj->getLine(line) ) {
	lineCt++;
	line.clear();
    }

    EXPECT_EQ(50787, lineCt);    
}
