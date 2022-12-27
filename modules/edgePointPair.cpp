#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include "utility.hpp"
#include "edgePointPair.hpp"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef K::Intersect_2 Intersect;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;
typedef CGAL::CartesianKernelFunctors::Intersect_2<K> Intersect;

edgePointPair::edgePointPair(int thePosition,Point current, Segment newSegment){
    point = current;
    segment = newSegment;
    position = thePosition;
    area=triangularAreaCalculation(current,newSegment.point(0),newSegment.point(1));
}

const double edgePointPair::getArea(){
    return this->area;
}
const Point edgePointPair::getPoint(){
    return this->point;
}
const Segment edgePointPair::getSegment(){
    return this->segment;
}
const int edgePointPair::getPosition(){
    return this->position;
}