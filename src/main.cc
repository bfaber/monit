#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
#include "logreader.h"
#include "csvprocessor.h"

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
    // parse options
    // works with both -h 192.0.0.1 and -h=192.0.0.1 
    while(1) {
	static struct option long_options[] = {
	       {"mongo_host", required_argument, 0, 'h'},
	       {"mongo_port", required_argument, 0, 'p'},
	       {"coll_name", required_argument, 0, 't'},
	       {"db_name", required_argument, 0, 'd'},
	       {"logfile", required_argument, 0, 'l'},
	       {"regex", required_argument, 0, 'r'},
	       {"csv", required_argument, 0, 'c'},
	       {0,0,0,0}
	};

	int opt_index = 0;
	option = getopt_long(argc, argv, "h:p:l:r:c:", long_options, &opt_index);

	if(option == -1)
	    break;

	switch(option) {
	  case 'h':
	      printf("got host: %s\n", optarg);
	      mongohost = std::string(optarg);
	      break;
	  case 't':
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
	  case 'c':
	    printf("csv: %s\n", optarg);
	    csv = std::string(optarg);
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

    // pull config from the db.
    auto *mi = new MongoInterface(mongohost, mongoport, dbName);
    auto *ms = new MongoSpooler(mi);

    std::vector<ConfigItem*> *configs = mi->getConfigs(configCollectionName);

    // this setup allows easier refactor to have multiple log readers per log processor
    // actually only have one log reader but have it handle multiple files
    // and multiple regexes. 
    auto *lp = new CSVProcessor(ms);
    auto *lr = new LogReader(configs, lp);

    auto lrTh = LogReader::start(lr); // fires off logReader thread for that instance
    auto lpTh = CSVProcessor::start(lp);

    lrTh.join(); // this should clean up first, then the processor thread
    lpTh.join();
    
    
    //    CSVProcessor::start(lp); // fires off processor thread for processing events
    delete lp;
    delete lr;
    
    return 0;
}
