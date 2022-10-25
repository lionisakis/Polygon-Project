#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL
make
./main -i input.txt -o output.txt -algorithm convex_hull -edge_selection 3