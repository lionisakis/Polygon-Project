#!/bin/bash

#  cgal_create_CMakeLists -s main
 cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=RELEASE
 make
 ./main -i theFolder2 -o output2.txt -preprocess 1
