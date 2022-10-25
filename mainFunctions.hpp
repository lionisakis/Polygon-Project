#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Polygon_2<K> Polygon;


void incremental(Polygon* ,std::vector<Point>* , int , int ,double* );
void convexHull(Polygon* , std::vector<Point>* , int , double* );

