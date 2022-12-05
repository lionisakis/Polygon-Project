#!/bin/bash

 cgal_create_CMakeLists -s main
 cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=RELEASE
 make
if [ $1 -eq 1 ] 
then
    ./main -i ./instances/data/images/euro-night-0000020.instance -o output.txt -algorithm local_search -L 10 -min -threshold 1 -algorithm2 incremental -edgeselection 1 -initialization 1a
elif [ $1 -eq 2 ]
then
    ./main -i ./instances/data/images/euro-night-0000400.instance -o output.txt -algorithm simulated_annealing -L 1000 -min -annealing global -algorithm2 convex_hull -edgeselection 1 -m 100
fi