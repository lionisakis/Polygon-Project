# Polygon-Project
authors: 
- Liarou Elena (sdi1900100)
- Lionis Emmanouil Georgios(Akis) (sdi1900103)

# Summary
This project takes a input file with coordinates of n integers points and creates a simple polygon based on either the incremental or convex hull algorithm. 

# Report
There is a report file named "Report.md" that you can find our results and comments about them.

# How to run the code
There is bash script with the name `run.sh` that you can run or modify the arguments to run the code.
Or you can run the following commands:
```
cgal_create_CMakeLists -s main 
cmake -DCGAL_DIR=/usr/lib/CGAL 
make 
./main -i [path/input.txt] -o [path/output.txt] -algorithm [Algo] -edge_selection [1-3] -initialiazation [1a,1b,2a,2b]
```

If you have already comple it then you have to run only the command:
```
./main -i [path/input.txt] -o [path/output.txt] -algorithm [Algo] -edge_selection [1-3] -initialiazation [1a,1b,2a,2b]
```

1. The path/input.txt is a path to the input file that has the follwoing:
  - In the first two(2) lines has to start with hastag(#) wich will be ingorred.
  - In the second(2nd) line has to be the following: `# parameters "convex_hull":{"area": "X"}` where X is the area of the convex hull.
  - The rest lines have to have the `id Xid Yid` and they are seperated by tab("\t")

2. The path/output.txt is a path where to creeate and/or store the output of the program.
There are stored the following:
  - Polygon
  - Cordinates of the vertexis
  - Cordinates of the edges
  - Which algorithm was given
  - The area calculated by us
  - The area calculated by CGAL
  - The ratio
  - The time to finish the algorihm.

3. There are only 2 types of algorithm that you can use and are: "incremental", "convex_hull".

4. We can have 3 diffrent types of edge selecion. The options are:
  - The 1 is the random
  - The 2 is the minimum area
  - The 3 is the maximum area.

5. The initialiazation is ***only*** necessary for the ***incremental***. The options are:
  - The 1a sorts the points by decreasing order of "x".
  - The 1b sorts the points by increasing order of "x".
  - The 2a sorts the points by decreasing order of "y".
  - The 2b sorts the points by increasing order of "y".

***Important the command line arguements have to be given correctly and only with that order. The Initialiazation is need only for the incremental algorithm.***

# Files - Description
There are 3 diffrent files: main,mainFunctions and utility

### Main 
The main handles the input and then calls the appropriate algorithm function and then makes the output.

### Main Functions
    Here lies the incremental algorithm and convex hull algorithm.

#### Incremental
  The algorithm sorts the points according to initiallazation from the command line and creates the initial triangle. For the remaining points we create the Convex Hull and then we find the reachable edges of the Convex Hull and for each of those we find the according visible edge of the polygon. Then we choose based on the edge selection and we add the point to the polygon with the new edge.

#### Concex Hull

### Utility
We have implemented some helpful functions that both algorithm make use of. These functions that may be used from the other algorithms are: coordinates Sorting, visible Edge Selector, swap, check Red, check Visibility. The local function is triangular Area Calculation. More explanation about what they do can be found in the file utility.cpp.
