Assumptions:
1) We're getting reading for every second. if suddenly you see and input for >60 seconds from now, We would have to mofidy the code to set everything previous to 0 ie call reset on timediff of the following array elements.

2) Well formatted input file, it always has seconds. milliseconds timestamp with a millisecond latency, that arrive in ascending order (as per the problem statement)
3) We're just reading inputs from a single file, we would have to introduce a read/write lock for inserting into a single StatsBuffer from multiple streaming inputs
4) latency is +ve. (we're resetting to 0)

Usage:

./StatsProcessor "/Users/pshah/Desktop/StatsProcessor/Sample.txt"

(note that the file path is a required arg)
This will read timestamp/latency inputs from the Sample.txt files until ctrl+c is called.
