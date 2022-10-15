# Polygon-Project

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL
make
./main

cgal_create_CMakeLists -s utility
cmake -DCGAL_DIR=/usr/lib/CGAL
make
./utility