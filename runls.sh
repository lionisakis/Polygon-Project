#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i ./instances/data/images/euro-night-0000200.instance -o output.txt -algorithm local_search -L 10 -max -threshold 10000 -algorithm2 incremental -edgeselection 2 -initilization 1a