#!/usr/bin/python

import logging
import json
import uuid
import time

filename = '/Users/bfaber/Desktop/play/cpp/monit/test/testing.log'
logging.basicConfig(filename='/Users/bfaber/Desktop/play/cpp/monit/test/testing.log', level=logging.DEBUG, format='%(asctime)s %(levelname)s %(message)s')

print "Start Log..."
numLines = 100
for i in range(0,numLines):
    anId = str(uuid.uuid1())
    someObj = { "userId": anId }
    someObj["requestId"] = anId
    someObj["machineId"] = "4azzz58ib"
    jsonObj = json.dumps(someObj)    
    #logging.warn("warning! REQUEST: %s", jsonObj)
    logging.debug('%s', jsonObj)
    print "logging stmt: " + str(jsonObj)
#    time.sleep(.001)
    
lineCt = 0
logfile = open(filename, 'r')
for l in logfile:
    lineCt += 1


print "Done Logging, logged " + str(lineCt) + " lines"
