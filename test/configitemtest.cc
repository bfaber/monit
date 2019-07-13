#include "configitem.h"
#include "gtest/gtest.h"

class ConfigItemTest : public ::testing::Test {
public:

    ConfigItem *ci;

    void SetUp() override {
	ci = new ConfigItem();
	ci->setName("testLogReader");
	ci->setRegex("^.*requestId\\\"\:\\\"(RQ[a-z0-9]{40}).*$");
	ci->setFileName("/Users/bfaber/Desktop/persephony/50kATC401.dat");
	ci->setCsv("requestId");
    };
};

// This test to run before 'isValid'
TEST_F(ConfigItemTest, isNotValid) {
    EXPECT_FALSE(ci->isValid());
}

// This test to run after 'isNotValid'
TEST_F(ConfigItemTest, isValid) {
    ci->setCollectionName("testLogReader");
    EXPECT_TRUE(ci->isValid());
}


