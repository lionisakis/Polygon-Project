#!/bin/bash

# cgal_create_CMakeLists -s main
# cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
# make
echo $1
if [ $1 -eq 1 ] 
then
    ./main -i ./instances/data/images/euro-night-0000200.instance -o output.txt -algorithm local_search -L 10 -max -threshold 10000 -algorithm2 incremental -edgeselection 2 -initilization 1a
elif [ $1 -eq 2 ]
then
    ./main -i ./instances/data/images/euro-night-0000200.instance -o output.txt -algorithm simulated_annealing -L 1000 -max -annealing subdivision -algorithm2 incremental -edgeselection 2 -initialization 2a -m 60
fi