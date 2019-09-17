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

# start the logreading proc first, then the log gen proc
logReadProc = subprocess.Popen("src/monit -h 127.0.0.1 -p 27017 -d test -c testConfig", shell=True)
print "logread is open"
logGenProc  = subprocess.Popen("test/logGenerator.py", stdout=subprocess.PIPE, shell=True)
print "loggen is open"
try:
    outs, errs = logGenProc.communicate(timeout=5)

    for l in outs:
        if "Done Logging, logged" in l:
            p = re.compile("\d+")
            val = p.findall(l)[0]
            print "lines parsed: " + val
        
    print "sleeping 3 seconds..."
    time.sleep(3)
    logReadProc.kill()
    print "killed monit"

except TimeoutExpired:
    logGenProc.kill()
    logReadProc.kill()
    outs, errs = logGenProc.communicate()




