#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
#include <memory>
#include <cstdio>

#include "logreadernew.h"
#include "mongorecordprocessor.h"
#include "logreaderexecutor.h"
#include "processorexecutor.h"
#include "pcre.h"

int main(int argc, char** argv) {
    printf("Parsing options...\n");
    int option;
    std::string mongohost;
    int mongoport;
    std::string configCollectionName;
    std::string dbName;
    std::string logfile;
    std::string regex;
    std::string csv;
    int transactionSize;
    
    // parse options
    // works with both -h 127.0.0.1 and -h=127.0.0.1 
    while(1) {
	static struct option long_options[] = {
	       {"mongo_host", required_argument, 0, 'h'},
	       {"mongo_port", required_argument, 0, 'p'},
	       {"db_name", required_argument, 0, 'd'},
	       {"coll_name", required_argument, 0, 'c'},
	       {"logfile", optional_argument, 0, 'l'},
	       {"regex", optional_argument, 0, 'r'},
	       {"csv", optional_argument, 0, 's'},
	       {"txn_size", optional_argument, 0, 't'},
	       {0,0,0,0}
	};

	int opt_index = 0;
	option = getopt_long(argc, argv, "s:d:h:p:l:r:c:t:", long_options, &opt_index);

	if(option == -1)
	    break;

	switch(option) {
	  case 'h':
	      printf("got host: %s\n", optarg);
	      mongohost = std::string(optarg);
	      break;
	  case 'c':
	      printf("got collection name: %s\n", optarg);
	      configCollectionName = std::string(optarg);
	      break;
	  case 'd':
	      printf("got db name: %s\n", optarg);
	      dbName = std::string(optarg);
	      break;
     	  case 'p':
	      mongoport = atoi(optarg);
	      printf("got port: %d\n", mongoport);
	      break;
	  case 'l':
	    printf("logfile: %s\n", optarg);
	    logfile = optarg;
	    break;
	  case 'r':
	    printf("regex: %s\n", optarg);
	    regex = std::string(optarg);
	    break;
	  case 's':
	    printf("csv: %s\n", optarg);
	    csv = std::string(optarg);
	    break;
	  case 't':
	    printf("txn_size: %s\n", optarg);
	    transactionSize = atoi(optarg);
	    break;

	  default:
	    break;
 	}
    }

    // memory here has to be freed by us at some point
    // would be good to wrap this in something that makes that obvious.
    //std::vector<const char*> events;
    
    // auto promise = std::promise<ReaderPromise>();
    // auto producer = std::thread([&] {
    //   lr->readFile(promise);
    // }
    // auto future = promise.get_future();
    // auto consumer = std::thread([&] {
    //   lp->start(future);
    // }
    // // block until complete
    // producer.join();
    // consumer.join();
    int result = 0;
    pcre_config(PCRE_CONFIG_JIT, &result);
    if(result == 1) {
	printf("we have jit!\n");
    } else {
	printf(" we dont have jit!\n");
    }
    

    // TODO: MongoInterface should implement a StorageInterface, which is then
    // instantiated here based on command line args.
    // StorageInterface implements at the least a getConfigs method.
    // pull config from the db.
    auto mi = std::make_unique<MongoInterface>(mongohost, mongoport, dbName, transactionSize);

    std::vector<ConfigItem*> *configs;
    try {
	configs = mi->getConfigs(configCollectionName);
    } catch(const std::runtime_error &re) {
	printf("%s\n", re.what());
	return(1);
    }

    if( configs->size() == 0 ) {
	printf("ERROR: no parseable config info in %s collection\n", configCollectionName.c_str());
	return 0;	
    }

    // TODO:  does MongoSpooler have any relationship to Mongo?  Could it just be a Spooler that is
    // instantiated with a StorageInterface?
    // init mongo interface first, then move it to spooler
    auto ms = std::make_unique<MongoSpooler>(std::move(mi));

    // TODO:  ditto here, what does this record processor have to do with Mongo?
    // this setup allows easier refactor to have multiple log readers per log processor
    // actually only have one log reader but have it handle multiple files
    // and multiple regexes.
    // NOTE: every time a shared ptr is shared, the ref ct is ATOMICALLY incremented/dec.
    //  not that important here, but elsewhere it will be.
    auto rp = std::make_shared<MongoRecordProcessor>(std::move(ms));
    auto lr = std::make_unique<LogReaderNew>(configs, rp);

    auto *rpTh = new ProcessorExecutor(rp);
    auto *lrTh = new LogReaderExecutor(std::move(lr));

    std::thread startedTh = ProcessorExecutor::start(rpTh);
    std::thread startedLrTh = LogReaderExecutor::start(lrTh);

    startedTh.join();
    startedLrTh.join();
    // two threads, one to read the files and one to processmatches and send to mongo.
    // the mongo match consumer thread starts first.
    // thread executors manage starting and stopping of threads here. 
    //    lr->readFiles(); //logreaderexecutor thread running, probably after the mongo/matchprocessor thread is running
    //    rp->processMatches();
    //    ms->commitToMongo();
    return 0;
}
