#!/bin/bash

cgal_create_CMakeLists -s utility
cmake -DCGAL_DIR=/usr/lib/CGAL
make
./utility