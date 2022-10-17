#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "utility.hpp"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;


void incremental(Polygon* polygon,vector<Point>* points, int sorting, int edge){
    // initialize the vector with size for complexity 
    vector<Segment> redSegment(points->size());
    vector<Segment> reachable(points->size());
    
    // make the triangle and short the points
    coordinatesSorting(polygon,points,sorting);
    for (int i=3;i<points->size();i++){
        Point currentPoint= points->at(i);
        Point previousPoint=points->at(i-1);
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            
        }
    }
    
}