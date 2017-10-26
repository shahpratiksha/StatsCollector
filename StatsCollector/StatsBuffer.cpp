//
//  StatsBuffer.cpp
//  Created by pshah on 10/25/17.
//  Copyright © 2017 pshah. All rights reserved.
//

#include <stdio.h>
#include "StatsBuffer.h"
#include <time.h>
#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

int StatsBuffer::update(int pos, time_t timestamp, float latency) {
    if (pos >= NUM_MINUTES) {
        perror("Trying to update invalid position for the circular array");
        return -1;
    }
    if (DEBUG_LOGGING == 1) {
        cout << "updating pos: " << pos << " latency: " << latency <<"\n";
        printAtPosition("before update", pos);
    }
    
    if (elements[pos][MIN] > latency || elements[pos][MIN] == 0 )
        elements[pos][MIN] = latency; // handle min
    
    if (elements[pos][MAX] < latency)
        elements[pos][MAX] = latency; // handle max
    
    elements[pos][COUNT]++; //count = count+1
    elements[pos][AVG] = (elements[pos][AVG] * (elements[pos][COUNT] -1) + latency )/elements[pos][COUNT];
    
    if (DEBUG_LOGGING == 1)
        printAtPosition("after update", pos);
    return 0;
}

int StatsBuffer::reset(int pos) {
    if (pos >= NUM_MINUTES) {
        perror("Trying to reset invalid position for the circular array");
        return -1;
    }
    for (auto i=0; i<NUM_STATS; ++i) {
        elements[pos][i] = 0; //Assuming: latency is +ve
    }
    return 0;

}

void StatsBuffer::insert(time_t timestamp , float latency) {
  
    if (DEBUG_LOGGING == 1)
        cout << "size: " << size << " tail: " << tail << " head: " << head << "\n";
    
    if (earliestTimestampForCurrentSlot == 0){
        head=0;
        tail=0;
        update(tail, timestamp, latency);
        size++;
        earliestTimestampForCurrentSlot = timestamp;
        return;
    }
    // we can insert into existing "tail" slot
    if (difftime(timestamp, earliestTimestampForCurrentSlot) < 60){
        if (DEBUG_LOGGING == 1)
            cout << "inserting into pos:" <<tail << "timestamp-latest=" << timestamp << "\n";
        assert(tail<NUM_MINUTES);
        update(tail, timestamp, latency);
        return;
    }
    
    //we need to insert into a new "tail" slot
    if (size < NUM_MINUTES)
        tail++;
    else {
        if (DEBUG_LOGGING == 1) cout << "reset head" << "\n";
        if (reset(head) == -1)
            perror("Tried to insert into an invalid position");
        tail = head;
        head++;
    }
    size++;
    if (DEBUG_LOGGING == 1)
        cout << "Inserting data for:" << timestamp << " . Earliest for this slot was:" << earliestTimestampForCurrentSlot << "\n";
    update(tail, timestamp, latency);
    earliestTimestampForCurrentSlot = timestamp;
}

void StatsBuffer::printAtPosition(string state, int pos) {
    cout << state << " " << elements[pos][MIN]
                << " " << elements[pos][MAX]
                << " " << elements[pos][AVG]
                << " " << elements[pos][COUNT] << "\n"; //TODO: don't print count
}

// We want to print the circular array in reverse order
void StatsBuffer::printContentsInReverse(){
    
    // The circular array still follows the normal ordering ie it hasn't wrapped around yet
    if (head < tail) {
        for (int i=tail, count=0; i>= head; i--,count++){
            cout << 0-count;
            printAtPosition(" ", i);
        }
        return;
    }
    
    // for a wrapped around array, print it in 2 parts:
    // part-1: starting from tail to 0
    for (int i=tail, count=0; i >=0; i--,count++){
        cout << 0-count;
        printAtPosition(" ", i);
    }
    
    // part-2 From end to head
    // note : you can't print end to tail because you could have missing elements between tail and head
    for (int i=NUM_MINUTES-1, count=0; i >=head; i--, ++count){
        cout << 0-head-count ;
        printAtPosition(" ", i);
    }
}
