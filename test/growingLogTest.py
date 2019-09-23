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
query = {"matchItemName":"movingLog"}
docs = testConfigCollection.find(query)
t1 = time.time()
print "DT get config: " + str(t1 - t0)
for doc in docs:
    print(doc)

    regex = doc['regex']
    file  = doc['filename']
    resultsCollection = doc['collectionName']

t0 = time.time()
lineCt = 0
# start the logreading proc first, then the log gen proc
logReadProc = subprocess.Popen("src/monit -h 127.0.0.1 -p 27017 -d test -c testMovingConfig", shell=True)

print "logread is open"
logGenProc  = subprocess.Popen("test/logGenerator.py", stdout=subprocess.PIPE, shell=True)
print "loggen is open"

stdout, stderr = logGenProc.communicate(timeout=5)
print "done comm"
line = ""
for l in stdout:
    line += l
    if l == '\n':
        print line.rstrip()
        if "Done Logging, logged" in l:
            p = re.compile("\d+")
            lineCt = p.findall(l)[0]
            print "lines parsed: " + lineCt

print "sleeping 1 second..."
time.sleep(1)
logReadProc.kill()
print "killed monit"

# how to validate:
# compare db.collection.count() to the line count above
dbCount = testdb["testMovingData"].count()
if lineCt == dbCount:
    print "TEST SUCCESSFUL!"
else:
    print "TEST UNSUCCESSFUL!"
    print "parsed line count: " + str(lineCt)
    print "db line count: " + str(dbCount)

# cleanup:
testdb["testMovingData"].delete_many({})
dbCount = testdb["testMovingData"].count()
if dbCount != 0:
    print "delete testMovingData data failed"
else:
    print "cleanup successful"

