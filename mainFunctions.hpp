#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Segment_2<K> Segment;


int incremental(Polygon* ,std::vector<Point>* , int , int ,double* , int flagSub=0,Segment* leftRight=NULL);
void convexHull(Polygon* , std::vector<Point>* , int , double*, int flagSub=0);

