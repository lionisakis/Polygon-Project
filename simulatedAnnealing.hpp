#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;

void simulated_annealing(Polygon* , int , double , int* ,int , int , int , int);
void subdivision(Polygon* , std::vector<Point>* , int , double , int *, int , int , int , int , int, int*);
void globalStep(Polygon* , int , double , int* , int , int , int , int flagSub=0);
void localMinimum(Polygon* ,int , double , int* , int , int , int );
