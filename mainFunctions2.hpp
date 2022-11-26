#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;


void localSearch(Polygon* , int , int , int , int*,int );
void simulated_annealing(Polygon* , int , double , int* ,int , int , int , int);
void subdivision(Polygon* polygon, std::vector<Point>* points, int typeOfOptimization, int L, int *finalArea, int countPoints, int chArea, int greedyAlgo, int greedyEdge, int m);