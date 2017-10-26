//
//  StatsCollector.h
//  Created by pshah on 10/21/17.
//  Copyright © 2017 pshah. All rights reserved.
//

#ifndef StatsCollector_h
#define StatsCollector_h

#include <fstream>
#include <iostream>
#include <signal.h>
#include "CircularArr.h"
#include <unistd.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>

using namespace std;

/*TODO make reusable by network socket*/
class InputStream{
public:
    InputStream(CircularArr *_res): results(_res) {}
    void flushResults() {return results->printContentsInReverse();}
    virtual void streamingHandler(string& filename);
    
private:
    CircularArr *results; // does not own
};

class FileInputStream /*:public InputStream*/{
public:
    FileInputStream(string _filename, CircularArr* _results):filename(_filename), results(_results) {
        ifstream f(filename);
    }
    int check_error_bits(ifstream *f);
    long long int getSizeAndSeek(ifstream& f, string path, long long int offset);
    void streamingHandler(string& logFile);
private:
    ifstream f;
    string filename;
    CircularArr *results; // does not own
    
};








#endif /* StatsCollector_h */
