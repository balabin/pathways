#!/bin/tcsh

# remove "-static" and "-lpthread" for compiling dynamically

g++ -static -c -O2 *.cpp -lboost_graph -lboost_regex -lm -lpthread
g++ -static -o pathcore -O2 *.o -lboost_graph -lboost_regex -lm -lpthread

exit 0
