#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i /Users/elenaliarou/Documents/term7/project/instances/data/images/euro-night-0000100.instance -o output.txt -algorithm simulated_annealing -L 1000 -min -annealing subdivision -algorithm2 convex_hull -edgeselection 2 -m 10