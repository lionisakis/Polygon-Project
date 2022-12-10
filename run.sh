#!/bin/bash

 cgal_create_CMakeLists -s main
 cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=RELEASE
 make
if [ $1 -eq 1 ] 
then
    ./main -i ./instances/data/images/euro-night-0000100.instance -o output.txt -algorithm local_search -L 10 -max -threshold 1 -algorithm2 incremental -edgeselection 3 -initialization 1a
elif [ $1 -eq 2 ]
then
    ./main -i ./instances/data/images/euro-night-0000100.instance -o output3-1.txt -algorithm simulated_annealing -L 1000 -max -annealing subdivision -algorithm2 incremental  -edgeselection 1 -initialization 1a 
    ./main -i ./instances/data/images/euro-night-0000100.instance -o output3-2.txt -algorithm simulated_annealing -L 1000 -max -annealing subdivision -algorithm2 convex_hull  -edgeselection 1 
    ./main -i ./instances/data/images/euro-night-0000400.instance -o output3-3.txt -algorithm simulated_annealing -L 1000 -max -annealing subdivision -algorithm2 incremental  -edgeselection 1 -initialization 1a 
    ./main -i ./instances/data/images/euro-night-0000400.instance -o output3-4.txt -algorithm simulated_annealing -L 1000 -max -annealing subdivision -algorithm2 convex_hull  -edgeselection 1 
    ./main -i ./instances/data/uniform/uniform-0000400-1.instance -o output3-5.txt -algorithm simulated_annealing -L 1000 -max -annealing subdivision -algorithm2 convex_hull  -edgeselection 1 
    ./main -i ./instances/data/images/euro-night-0001000.instance -o output1-6.txt -algorithm simulated_annealing -L 1000 -max -annealing local -algorithm2 convex_hull  -edgeselection 1  
fi