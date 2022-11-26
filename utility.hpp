
#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "edgeChange.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;


void coordinatesSorting(Polygon* ,std::vector<Point>* ,int ,double* );
Segment visibleEdgeSelector(Point ,std::vector<Segment>* , int ,double* );
void swap(Point* , Point* ,int );
int checkRed(std::vector<Point>* , Point , Point );
int checkVisibility(Polygon* , Point , Point );
double triangularAreaCalculation(Point , Point , Point );
double calculateNewArea(Polygon* , Segment , Point , Point , std::vector<Point>* );
double maxEnergy(int  , int , int );
double minEnergy(int  , int , int );
void changeEdge(Polygon* ,EdgeChange*, int );
int Metropolis(double DE,double T);
void sortPoints(std::vector<Point>* , int );