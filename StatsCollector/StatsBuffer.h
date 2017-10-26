//
//  StatsBuffer.h
//  Created by pshah on 10/23/17.
//  Copyright © 2017 pshah. All rights reserved.
//

#ifndef StatsBuffer_h
#define StatsBuffer_h

#include <stdio.h>
#include <time.h>
#include <string>
#define DEBUG_LOGGING 0

using namespace std;

//TODO: implement locking

/* Remember to update corresponding stat enum when you add a new statistic */
#define NUM_STATS 4 // min, max, avg, count
#define NUM_MINUTES  60

/* Remember to update the NUM_STATS count while adding another statistic */
enum stat {
    MIN,
    MAX,
    AVG,
    COUNT
};

/* TODO: make this reusable:
 * 1) templatize float
 * 2) make update/insert more generic
 * 3) */

class StatsBuffer {
    //TODO: change to constructor taking 60,4 
public:
    int update(int pos, time_t timestamp, float latency);
    int reset (int pos);
    void insert(time_t timestamp, float latency);
    void printAtPosition(string state, int pos); //for debugging
    void printContentsInReverse();

private:
    int head=0, tail=-1, size=0;
    float elements[NUM_MINUTES][NUM_STATS]; // min, max, avg, count, in that order
    time_t earliestTimestampForCurrentSlot=0;
};


#endif /* StatsBuffer_h */
