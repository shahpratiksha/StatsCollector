//
//  StatsCollector.h
//  Created by pshah on 10/21/17.
//  Copyright © 2017 pshah. All rights reserved.
// StreamProcessor
//parser / validator


#ifndef StatsCollector_h
#define StatsCollector_h

#include <fstream>
#include <iostream>
#include <signal.h>
#include "CircularArr.h"
#include <unistd.h>


using namespace std;

//TODO: can you use a factory ? for polling based model
/*TODO make reusable by network socket*/
class InputStream{
public:
    //constructor is the setup method
    InputStream(CircularArr *_res): results(_res) {}
    
    void flushResults() {return results->printContentsInReverse();}
    //protected : rename to processData -> this validates the data and inserts
    virtual void streamingHandler(string& filename);
    //require implementor to implement read method
    
private:
    CircularArr *results; // does not own
    //VALIDATE DATA
};

// getline gets a record -> inputstream processes record 
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
