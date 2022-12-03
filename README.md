# Polygon-Project
github link: https://github.com/lionisakis/Polygon-Project

authors: 
- Liarou Elena (sdi1900100)
- Lionis Emmanouil Georgios(Akis) (sdi1900103)

# Summary
This project takes an input file with coordinates of n integers points and creates a simple polygon based on either the incremental or convex hull algorithm. Then it optimizes the polygon created by the greedy either by applying local search or simulated annealing. 

# Report
There is a report file named "Report.md" where you can find our results and comments.

# How to run the code
The command line arguments must be given strictly in the order described below. 
 
# How to run the code for Local Search
***run command: ./run.sh 1***
There is a bash script with the name `run.sh` that you can run or modify the arguments to run the code.
Alternatively,  it can be run by following commands:
```
cgal_create_CMakeLists -s main 
cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=RELEASE
make 
./main -i [path/input.txt] -o [path/output.txt] -algorithm local_search -L [1,10] -[min/max] -algorithm2 [incremental/convex_hull] -edge_selection [1,2,3] -initialization [1a,1b,2a,2b] (initialization is required only for the incremental)
```

If you have already completed it, then you have to run only the command:
```
./main -i [path/input.txt] -o [path/output.txt] -algorithm local_search -L [1,10] -[min/max] -algorithm2 [incremental/convex_hull] -edge_selection [1,2,3] -initialization [1a,1b,2a,2b] (initialization is required only for the incremental)
```

# How to run the code for Simulated annealing
***run command: ./run.sh 2***
There is a bash script with the name `run.sh` that you can run or modify the arguments to run the code.
Alternatively,  it can be run by following commands:
```
cgal_create_CMakeLists -s main 
cmake -DCGAL_DIR=/usr/lib/CGAL -DCMAKE_BUILD_TYPE=RELEASE
make 
./main -i [path/input.txt] -o [path/output.txt] -algorithm simulated_annealing -L [L>=1000] -[min/max] -annealing [local/global/subdivision] -algorithm2 [incremental/convex_hull] -edge_selection [1-2-3] -initialization [1a,1b,2a,2b] -m [10,100] (initialization only for incremental), (m only for annealing=subdivision)
```

If you have already completed it, then you have to run only the command:
```
./main -i [path/input.txt] -o [path/output.txt] -algorithm simulated_annealing -L [L>=1000] -[min/max] -annealing [local/global/subdivision] -algorithm2 [incremental/convex_hull] -edge_selection [1-2-3] -initialization [1a,1b,2a,2b] -m [10,100] (initialization only for incremental), (m only for annealing=subdivision)
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

3. There are only 2 types of greedy algorithms that you can use, which are: "incremental", "convex_hull".

4. We can have 3 different types of edge selecion. The options are:
  - The 1 is the random
  - The 2 is the one adding minimum area
  - The 3 is the one maximum area.

5. The Initialization is ***only*** necessary for the ***incremental***. The options are:
  - The 1a sorts the points by decreasing the order of "x".
  - The 1b sorts the points by increasing the order of "x".
  - The 2a sorts the points by decreasing the order of "y".
  - The 2b sorts the points by increasing the order of "y".

6. There are only 2 types of optimization algorithms that you can use, which are: "local search", "simulated annealing".

7. There are two types of optimization either max or min. 

8. Command line argument ***m**** is required ***only*** for ***simulated annealing with subdivion***, has to be greater than 10 and less than 100. 

***Important the command line arguments have to be given correctly and only in that order. The Initialization is needed only for the incremental algorithm.***

# Files - Description
There are 6 different code files: main,mainFunctions, mainFunctions2, edgeChange, edgePointpair and utility.
The last 5 have header files of the same name, in these header files can be found the definions of the functions implemented in the code files and the #include and typedef that are necessary for these functions. 

### Main 
The main handles the input and then calls the appropriate algorithm function, and then makes the output.

### Main Functions
    Here lies the incremental algorithm and convex hull algorithm.

#### Incremental
  The algorithm sorts the points according to Initialization from the command line and creates the initial triangle. For the remaining points, we create the Convex Hull, and then we find the reachable edges of the Convex Hull, and for each of those, we find the according visible edge of the polygon. Then we choose based on the edge selection, and we add the point to the polygon with the new edge.

#### Concex Hull
The algorithm creates the convex hull of the polygon and initializes the polygon based on that, then until there are no internal points left, for each edge of the polygon we find its closest point(for this point the edge must be visible) and based on the edge selection we choose which edge we are going to break.

### Main Functions2
    Here lies the implementation of the local search algorithm and the simulated annealing with all 3 different versions(local step, global step and subdivison). Local search and  simulated annealing with global or local step take as input a polygon already constructed by one of the previous greedy algorithms(incremental or convex-hull) for all n starting points. Whereas simulated annealing with subdivision does not require that. Which greedy algorithm is to be used has to be specified in the command line arguments. Function checkPath is also implemented here, it checks if a given path and edge u1u2 is a valid change, returns 0 for valid and 1 for invalid. 

#### Local Search
  The algorithm finds for every edge of the current polygon all valid paths with length <=L(L is given in the command line), the change is implemented as such: "Move path V = v1,...,vk, of k ≥ 1 vertices (in reverse order) between the endpoints of u1u2. This edge is replaced by a detour to include V.The 2 prior edges linking the chain to V are replaced by a single edge (top)." A path is considered valid if the change retains the simplicity of the polygon. We store all possible changes for the current polygon in a vector of type edgeChange, we choose which change we are going to implement based on the typeOfOptimization(max or min, given in command line arguments). For files with a lot of starting points n>50, we randomly choose 10 edges and search valid paths only for these 10 edges, as the algorithm in case we want to search for all edges becomes really slow. The threshold musy also be given by the user in the command line arguments. Changes are applied while DA>threshold where DA is the abs(previousPolygon.area() - currentPolygon.area()).

#### Simulated Annealine with local step
  The algorithm randomly chooses a point q and we swap its position with its succesor t. Let p the point that precedes q, and s the point that succeeds r.The resulting polygon is valid iff (a) the new edges pr, qs do not intersect each other, (b) these edges do not intersect another edge in the chain. To check the validity of the change we make use of the kd Tree class from the CGAL library. We aplly the transition only when DE<0 or the Metropolis criterion is satisfied, where DE=prevEnerg-currEnergy. The total number of valid changes that have to be found and considered for impementation are L(given as command line argument from user).

#### Simulated Annealine with global step
  The algorithm randomly chooses 2 points q, s.  Let p, r be the points that precede and succeed q resp., and let t be the point after s. Connect p and r (removing q from the chain); insert q between s and t. The resulting polygon is valid iff (a) the new edge pr does not intersect the other new edges sq, qt, (b) each of these new edges does not intersect another edge. We aplly the transition only when DE<0 or the Metropolis criterion is satisfied, where DE=prevEnerg-currEnergy. The total number of valid changes that have to be found and considered for impementation are L(given as command line argument from user).

#### Simulated Annealine with subdivion
  The algorithm separates the n points in k=ceil([n-1]/[m-1]) subsets of approximately m points each(m is given from user in command line arguments). But we make sure hat in every subset’s Lower Hull, the rightmost edge (e.g. qp) and the leftmost edge (e.g. qr), except in the last or the first subset, resp., is strictly monotone increasing or decreasing resp. (by y). Before we create the k subsets we order the n points according to x increasing, and then divide them. For each subset a greedy algorithm is called to constuct the initial polygon(we have modified the incremental and convex hull so as to to maintain the leftmost and rightmost edge descriped above). For each of the polygons constructed we apply the simulated annealing with global step(again we maintain the afformentioned edges). After all k polygons have been created and optimized with the global step, we connect them into one polygon. At last, we apply simulated annealing with local step on this polygon. If incremental fails to create polygon with the marked edges included then convex hull is called. 



### edgePointPair
  Here can be found our implementation of class edgePointPair, which we use in the convex hull algorithm. In this class we store 1)an edge,  2) its closest point that has not been added to the polygon yet and for which the edge is reachable and 3) the area that we will have to subtrack from the polygon if we choose to break this edge.

### edgeChange
  Here can be found our implementation of class edgeChange, which we use in the local search algorithm. In this class we store 1)the edge u1u2 that is to be broken,  2,3) the first and last vertex of the path with length <=L that is valid for this edge> 4) the area that we will have to add/substract to the polygon if we choose to break this edge.


### Utility
We have implemented some useful functions that both algorithms make use of. These functions that may be used from the other algorithms are: coordinates Sorting, visible Edge Selector, swap, check Red, check Visibility, calculateNewArea, changeEdge, Metropolis and sortPoints. The local function is triangular Area Calculation. More explanation about what they do can be found in the file utility.cpp.


