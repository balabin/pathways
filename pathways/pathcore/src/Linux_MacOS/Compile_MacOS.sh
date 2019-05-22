#!/bin/tcsh

# remove "-static" and "-lpthread" for compiling dynamically

g++ -o pathcore -O2 -I/Users/ibalabin/local/boost_1_66_0 -L/Users/ibalabin/local/boost_1_66_0/stage/lib *.cpp /Users/ibalabin/local/boost_1_66_0/stage/lib/libboost_graph.a /Users/ibalabin/local/boost_1_66_0/stage/lib/libboost_regex.a -lm -lpthread

exit 0
