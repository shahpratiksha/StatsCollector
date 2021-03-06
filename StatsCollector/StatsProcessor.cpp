//
//  StatsProcessor.cpp
//  Created by splunker on 10/25/17.
//  Copyright © 2017 pshah. All rights reserved.



#include "StatsProcessor.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>

#define DEBUG_LOGGING 0

using namespace std;

/*check the status of the last getline()*/
int FileInputProcessor::check_error_bits(ifstream* f) {
    int retval = 3;
    if (f->eof()) {
        perror("stream eofbit. error state");
        retval = 0;
    }
    if (f->fail()) {
        perror("stream failbit (or badbit). error state");
        retval = 1;
    }
    if (f->bad()) {
        perror("stream badbit. error state");
        retval = 2;
    }
    return retval;
}

long long int FileInputProcessor::getSizeAndSeek(ifstream& f, string path, long long int offset) {
    f.open(path);
    f.seekg(0,f.beg);
    f.seekg (0, f.end);
    long long int retval = f.tellg();
    f.seekg(offset);
    return retval;
}

/* This function reads the file till EOF, and once it reaches the end,
 * sleeps for 2 seconds and tries to read the file again from where it
 * left off last time to the new EOF. It is a bit janky, since we have to
 * open and close the file multiple times, if the input stream is paused for
 * some reason. But, for a reasonable steady stream of incoming data,
 * works fine */
void FileInputProcessor::processData(const string& logFile) {
    ifstream f;
    string line;
    time_t ts;
    float ms, latency;
    long long int length = getSizeAndSeek(f,logFile,0);
    
    if (!f.is_open())
        perror("is_open() returned false. error state");
    else
        if (DEBUG_LOGGING == 1) cout << "is_open() returned true." << endl;
    
    while(1) {
        getline(f, line);
        //Caution: We're assuming sane input. No validation done here
        f >> ts >> ms >> latency;
        if (!f.eof()){
            if(DEBUG_LOGGING == 1)
                cout << "Parsed timestamp:" << ts << ", latency:" << latency
                << ". Inserting into the circular array" << "\n";
            results->insert(ts,latency);
        }
        else {
            f.clear();
            if (DEBUG_LOGGING == 1) cout << "Clearing eof flag" << "\n";
            f.close();
            long long int temp = getSizeAndSeek(f, logFile, length);
            length = temp;
            
            cout << "Sleeping for 2 secs, waiting for more data" << "\n";
            this_thread::sleep_for(chrono::seconds(2));
        }
    }
}
