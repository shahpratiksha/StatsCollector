//
//  main.cpp
//  Created by pshah on 10/21/17.
//  Copyright © 2017 pshah. All rights reserved.
//


#include <fstream>
#include "StatsProcessor.h"
#include "StatsBuffer.h"
#include <signal.h>

using namespace std;

//had to be declared as global for sigint :(
StatsBuffer results;

void my_handler(int s){
    printf("Caught signal %d\n",s);
    results.printContentsInReverse();
    exit(1);
}

void registerInterrupt() {
    // Setup printing on Ctrl+C
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}


int main(int argc, const char * argv[]) {
    
    string logFile;
    
    // Parse out the filename
    if (argc < 1) {
        cout << "Insufficient number of arguments. pass in file name using -f ";
        return -1;
    }
    
    logFile = argv[1];
    registerInterrupt();
    FileInputProcessor* fis = new FileInputProcessor(logFile, &results);
    fis->processData(logFile);
    
    return 0;
}
