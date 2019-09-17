# src/monit -h 127.0.0.1 -p 27017 -d test -c testConfig

import sys
import re
import time

import subprocess
from pymongo import MongoClient

# configuration for this testing is already in mongo under the 'testConfig' collection.
mongo = MongoClient('127.0.0.1', 27017)
testdb = mongo.test
testConfigCollection = testdb.testConfig

class Config:
    def __init__(self, regex, file, resultsCollection):
        self.regex = regex
        self.file = file
        self.resultsCollection = resultsCollection
        self.pythonFoundCount = 0
        
#{u'regex': u'^.*requestId\\"\\:\\"(RQ[a-z0-9]{40}).*$', u'collectionName': u'testReturnData', u'matchItemName': u'reqIds', u'filename': u'/Users/bfaber/Desktop/persephony/50kATC401.dat', u'_id': ObjectId('5d36293cef70041cd73e2464'), u'csv': u'requestId', u'proof': u'\\:'}
t0 = time.time()
docs = testConfigCollection.find()
t1 = time.time()
print "DT get config: " + str(t1 - t0)
configs = []
for doc in docs:
    print(doc)

    regex = doc['regex']
    file  = doc['filename']
    resultsCollection = doc['collectionName']

    configs.append(Config(regex, file, resultsCollection))

for c in configs:
    t0 = time.time()
    compiledRegex = re.compile(c.regex)
    f = open(c.file)
    matches = []
    for l in f.readlines():
        match = compiledRegex.match(l)
        if match != None:
            matches.append(match.group())
    t1 = time.time()

    c.pythonFoundCount = len(matches)
    print "python parsed the file: " + str(c.file)
    print "python parsed using this regex: " + str(c.regex)
    print "python found count: " + str(c.pythonFoundCount)
    print "python parse DT: " + str(t1 - t0)

# now run the system on same config
t0 = time.time()
monitProc = subprocess.Popen("src/monit -h 127.0.0.1 -p 27017 -d test -c testConfig", shell=True)
t1 = time.time()
print "monitProc running..."

print "sleeping 3 seconds..."
time.sleep(3)
monitProc.kill()
print "killed monit"

successCt = 0
# now go to db and get count from there
for c in configs:
    t0 = time.time()
    resultsCollectionObj = testdb[c.resultsCollection]
    count = resultsCollectionObj.count()
    resultsCollectionObj.delete_many({})
    t1 = time.time()
    print c.resultsCollection + " collection count: " + str(count)
    print c.resultsCollection + " collection count and clear DT: " + str(t1 - t0)
    
    if count == c.pythonFoundCount:
        successCt += 1
        
if successCt == len(configs):
    print "TEST SUCCESSFUL"
else:
    print "TEST FAILED"

print "cleaning up..."

# then clear that collection

print "done."
    
