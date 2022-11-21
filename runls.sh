#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i /Users/elenaliarou/Documents/term7/project/instances/data/images/euro-night-0000020.instance -o output.txt -algorithm local_search -L 6 -max -threshold 1000000