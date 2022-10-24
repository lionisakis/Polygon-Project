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


void incremental(Polygon* ,std::vector<Point>* , int , int ,double* );
void convexHull(Polygon* , std::vector<Point>* , int , double* );

class edgePointPair{
    public:
    Point nearestPoint;
    Segment edge;
    int area;

    edgePointPair(Point current, Segment newEdge){
        nearestPoint = current;
        edge = newEdge;
    }
    ~edgePointPair(){

    }
}