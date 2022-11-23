#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i /home/akis/Downloads/instances/data/images/euro-night-0000010.instance -o output.txt -algorithm simulated_annealing -L 1000 -min -annealing local