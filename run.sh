#!/bin/bash

cgal_create_CMakeLists -s utility
cmake -DCGAL_DIR=/usr/lib/CGAL
make
./main -i a.txt -o b.txt -algorithm convex_hull -edge_selection 2 