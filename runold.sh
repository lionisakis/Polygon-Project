
#!/bin/bash

cgal_create_CMakeLists -s main
cmake -DCGAL_DIR=/usr/lib/CGAL -CMAKE_BUILD_TYPE=RELEASE
make
./main -i /Users/elenaliarou/Documents/term7/project/instances/data/images/euro-night-0000400.instance -o output.txt -algorithm incremental -edge_selection 3 -initialization 2a