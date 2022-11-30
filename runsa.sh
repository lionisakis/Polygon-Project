#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i ./instances/data/images/euro-night-0000100.instance -o output.txt -algorithm simulated_annealing -L 1000 -min -annealing subdivision -algorithm2 incremental -edgeselection 2 -initialization 2a -m 30