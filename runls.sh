#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i /home/akis/Downloads/instances/data/images/euro-night-0000400.instance -o output.txt -algorithm local_search -L 1 -min 4 -threshold 100000