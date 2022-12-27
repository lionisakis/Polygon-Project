#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include "../include/geoUtil.hpp"
#include "../include/genericUtil.hpp"
#include "../include/edgeChange.hpp"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef K::Intersect_2 Intersect;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;
typedef CGAL::CartesianKernelFunctors::Intersect_2<K> Intersect;

EdgeChange::EdgeChange(Point l,Point r, Segment currentEdge, int newArea){
    left = l;
    right = r;
    segment = currentEdge;
    area = newArea;
}

const Point EdgeChange::getLeft(){
    return this->left;
}

const Point EdgeChange::getRight(){
    return this->right;
}

const double EdgeChange::getArea(){
    return this->area;
}

const Segment EdgeChange::getSegment(){
    return this->segment;
}