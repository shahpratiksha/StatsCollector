//
//  StatsProcessor.h
//  Created by pshah on 10/21/17.
//  Copyright © 2017 pshah. All rights reserved.
// StreamProcessor
//parser / validator


#ifndef StatsProcessor_h
#define StatsProcessor_h

#include <fstream>
#include <iostream>
#include <signal.h>
#include "StatsBuffer.h"
#include <unistd.h>


using namespace std;

//TODO: can you use a factory ? for polling based model

class InputProcessor{

public:
    InputProcessor(StatsBuffer *_res): results(_res) {}
    void flushResults() { return results->printContentsInReverse(); }

protected : // this validates the data and inserts
    virtual void processData(const string& filename)=0;
    //require implementor to implement read method
    StatsBuffer *results; // does not own
};

// getline gets a record -> InputProcessor processes record 
class FileInputProcessor :public InputProcessor{
public:
    FileInputProcessor(string _filename, StatsBuffer* _results):InputProcessor(_results) {
        filename = _filename;
        ifstream f(filename);
    }
    int check_error_bits(ifstream *f);
    long long int getSizeAndSeek(ifstream& f, string path, long long int offset);
    void processData(const string& logFile);
    bool processRecord(const string& line);
private:
    ifstream f;
    string filename;
};








#endif /* StatsProcessor_h */