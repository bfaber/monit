#include "gtest/gtest.h"

#include "configitem.h"
#include "matchbundle.h"
#include "record.h"

#include <vector>
#include <string>

class RecordTest : public ::testing::Test {

    void SetUp() override {};
    void TearDown() override {};


};

TEST_F(RecordTest, RecordCreationTest) {

    ConfigItem *ci = new ConfigItem();
    ci->setName("testRecord");
    ci->setRegex("some bs regex");
    ci->setFileName("some bs file");
    ci->setCsv("group1CollName,group2CollName");
    ci->setCollectionName("testCollectionName");

    MatchBundle *mb = new MatchBundle(ci);
    std::string group1 = "capturedgroup1";
    std::string group2 = "capturedgroup2";

    std::vector<std::string> groups;
    groups.push_back(group1);
    groups.push_back(group2);

    mb->addGroups(groups);
    ASSERT_EQ(1, mb->getBundle().size()); // number of matched lines from logfile
    EXPECT_EQ(2, mb->getBundle()[0].size()); // number of groups in match set
    

    std::vector<MatchBundle*> bundles;
    bundles.push_back(mb);
    
    Record *rec = new Record(bundles);
    ASSERT_EQ(1, rec->size());

    const bson_t *doc = rec->getDocs()[0];
    const char* json = bson_as_json(doc, nullptr);
    //printf("json string: %s\n", json);
    std::string csvstring = "{ \"group1CollName\" : \"capturedgroup1\", \"group2CollName\" : \"capturedgroup2\" }";
    EXPECT_EQ(csvstring, std::string(json));

    EXPECT_EQ("testCollectionName", rec->getCollectionName());      
}
