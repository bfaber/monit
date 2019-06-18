#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <vector>
#include "logreader.h"
#include "csvprocessor.h"

int main(int argc, char** argv) {
    printf("Parsing options...\n");
    int option;
    std::string logfile;
    std::string regex;
    std::string csv;
    // parse options
    // works with both -h 192.0.0.1 and -h=192.0.0.1 
    while(1) {
	static struct option long_options[] = {
	       {"mongo_host", required_argument, 0, 'h'},
	       {"mongo_port", required_argument, 0, 'p'},
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
	    break;
     	  case 'p':
	    printf("got port: %d\n", atoi(optarg));
	    break;
	  case 'l':
	    printf("logfile: %s\n", optarg);
	    logfile = optarg;
	    break;
	  case 'r':
	    printf("regex: %s\n", optarg);
	    regex = optarg;
	    break;
	  case 'c':
	    printf("csv: %s\n", optarg);
	    csv = optarg;
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


    // this setup allows easier refactor to have multiple log readers per log processor
    auto *lp = new CSVProcessor(csv.c_str());
    auto *lr = new LogReader(logfile.c_str(), regex.c_str(), lp);
    auto lrTh = LogReader::start(lr); // fires off logReader thread for that instance
    auto lpTh = CSVProcessor::start(lp);

    lrTh.join(); // this should clean up first, then the processor thread
    lpTh.join();
    
    
    //    CSVProcessor::start(lp); // fires off processor thread for processing events
    delete lp;
    delete lr;
    
    return 0;
}
