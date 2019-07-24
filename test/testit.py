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

file = ""
regex = ""
resultsCollection = ""

#{u'regex': u'^.*requestId\\"\\:\\"(RQ[a-z0-9]{40}).*$', u'collectionName': u'testReturnData', u'matchItemName': u'reqIds', u'filename': u'/Users/bfaber/Desktop/persephony/50kATC401.dat', u'_id': ObjectId('5d36293cef70041cd73e2464'), u'csv': u'requestId', u'proof': u'\\:'}
t0 = time.time()
docs = testConfigCollection.find({})
t1 = time.time()
print "DT get config: " + str(t1 - t0)
for doc in docs:
    print(doc)

    regex = doc['regex']
    file  = doc['filename']
    resultsCollection = doc['collectionName']

t0 = time.time()    
f = open(file)
compiledRegex = re.compile(regex)

matches = []
for l in f.readlines():
    match = compiledRegex.match(l)
    if match != None:
        matches.append(match)
t1 = time.time()
pythonFoundCount = len(matches)
print "python found count: " + str(pythonFoundCount)
print "python parse DT: " + str(t1 - t0)

# now run the system on same config
t0 = time.time()
returnVal = subprocess.call("src/monit -h 127.0.0.1 -p 27017 -d test -c testConfig", shell=True)
t1 = time.time()
print "return val: " + str(returnVal)
print "process DT: " + str(t1 - t0)
if returnVal != 0:
    print "ERROR starting or stopping or running monit"
    exit

# now go to db and get count from there
t0 = time.time()
resultsCollectionObj = testdb[resultsCollection]
count = resultsCollectionObj.count()
t1 = time.time()
print "results collection count: " + str(count)
print "results collection count DT: " + str(t1 - 10)

if count == pythonFoundCount:
    print "TEST SUCCESSFUL"
else:
    print "TEST FAILED"

print "cleaning up..."

# then clear that collection
resultsCollectionObj.delete_many({})
print "done."
    
