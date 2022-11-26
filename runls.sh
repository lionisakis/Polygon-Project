#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i /Users/elenaliarou/Documents/term7/project/instances/data/images/euro-night-0000400.instance -o output.txt -algorithm local_search -L 10 -max -threshold 10000 -algorithm2 incremental -edgeselection 1 -initilization 1a