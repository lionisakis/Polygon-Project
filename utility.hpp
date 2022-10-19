
#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;


void coordinatesSorting(Polygon* polygon,std::vector<Point>* points,int type,double* area);
Segment visibleEdgeSelector(Point newPoint,std::vector<Segment>* vector, int type,double* area);
int isItRed(std::vector<Point>* KP,Point a,Point b,Point p);
int isItReachable(Polygon* polygon,Point a,Point b,Point p);
int checkEdgeInsideRedLine(Segment polygonSegment,Segment redline,int sorting);
void swap(Point* a, Point* b,int type);