#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL
make
./main -i input.txt -o output.txt -algorithm incremental -edge_selection 2 -initialization 1a