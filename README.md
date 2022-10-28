# Polygon-Project
authors: 
- Liarou Elena (sdi1900100)
- Lionis Emmanouil Georgios(Akis) (sdi1900103)

# Summary
This project takes an input file with coordinates of n integers points and creates a simple polygon based on either the incremental or convex hull algorithm. 

# Report
There is a report file named "Report.md" where you can find our results and comments.

# How to run the code
There is a bash script with the name `run.sh` that you can run or modify the arguments to run the code.
Alternatively,  it can be run by following commands:
```
cgal_create_CMakeLists -s main 
cmake -DCGAL_DIR=/usr/lib/CGAL 
make 
./main -i [path/input.txt] -o [path/output.txt] -algorithm [Algo] -edge_selection [1-3] -initialization [1a,1b,2a,2b]
```

If you have already completed it, then you have to run only the command:
```
./main -i [path/input.txt] -o [path/output.txt] -algorithm [Algo] -edge_selection [1-3] -initialiazation [1a,1b,2a,2b]
```

1. The path/input.txt is a path to the input file that has the following:
  - In the first two(2) lines, it has to start with the hashtag(#), which will be ignored.
  - In the second(2nd) line has to be the following: `# parameters "convex_hull":{"area": "X"}` where X is the area of the convex hull.
  - The rest lines have to have the `id Xid Yid`, and they are separated by tab("\t")

2. The path/output.txt is a path where to create and/or store the output of the program.
There are stored the following:
  - Polygon
  - Coordinates of the vertexes
  - Coordinates of the edges
  - Which algorithm was given
  - The area calculated by us
  - The area calculated by CGAL
  - The ratio
  - Construction time of polygon in miliseconds.

3. There are only 2 types of algorithms that you can use, which are: "incremental", "convex_hull".

4. We can have 3 different types of edge selecion. The options are:
  - The 1 is the random
  - The 2 is the one adding minimum area
  - The 3 is the one maximum area.

5. The Initialization is ***only*** necessary for the ***incremental***. The options are:
  - The 1a sorts the points by decreasing the order of "x".
  - The 1b sorts the points by increasing the order of "x".
  - The 2a sorts the points by decreasing the order of "y".
  - The 2b sorts the points by increasing the order of "y".

***Important the command line arguments have to be given correctly and only in that order. The Initialization is needed only for the incremental algorithm.***

# Files - Description
There are 4 different files: main,mainFunctions, edgePointpair and utility.

### Main 
The main handles the input and then calls the appropriate algorithm function, and then makes the output.

### Main Functions
    Here lies the incremental algorithm and convex hull algorithm.

#### Incremental
  The algorithm sorts the points according to Initialization from the command line and creates the initial triangle. For the remaining points, we create the Convex Hull, and then we find the reachable edges of the Convex Hull, and for each of those, we find the according visible edge of the polygon. Then we choose based on the edge selection, and we add the point to the polygon with the new edge.

#### Concex Hull
The algorithm creates the convex hull of the polygon and initializes the polygon based on that, then until there are no internal points left, for each edge of the polygon we find its closest point(for this point the edge must be visible) and based on the edge selection we choose which edge we are going to break.


### edgePointPair
  Here can be found our implementation of class edgePointPair, which we use in the convex hull algorithm. In this class we store 1)an edge,  2) its closest point that has not been added to the polygon yet and for which the edge is reachable and 3) the area that we will have to subtrack from the polygon if we choose to break this edge.


### Utility
We have implemented some useful functions that both algorithms make use of. These functions that may be used from the other algorithms are: coordinates Sorting, visible Edge Selector, swap, check Red, and check Visibility. The local function is triangular Area Calculation. More explanation about what they do can be found in the file utility.cpp.
