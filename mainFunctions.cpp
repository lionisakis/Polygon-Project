#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include "utility.hpp"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;
typedef CGAL::CartesianKernelFunctors::Intersect_2<K> Intersect;

int checkVisibility(Polygon* p, Point newPoint, Point checkPoint){
    Segment raycast(newPoint, checkPoint);
    for(EdgeIterator ei=p->edges_begin();ei!=p->edges_end();ei++){
        if(const auto res = intersection(raycast, ei))
            return 0;
    }
    return 1;
}


void incremental(Polygon* polygon,vector<Point>* points, int sorting, int edge){
    // initialize the vector with size for complexity 
    vector<Segment> reachable(points->size());
    
    // make the triangle and short the points
    coordinatesSorting(polygon,points,sorting);
    for (int i=3;i<points->size();i++){
        Point currentPoint= points->at(i);
        Point previousPoint=points->at(i-1);
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            if(ei->point(1) == previousPoint){//we found the left red line 
                EdgeIterator ei2=ei;
                while(ei!=polygon->edges_begin()){//check for reachable edges from left side of preivous point
                    if(checkVisibility(polygon, currentPoint, ei->point(0)) && checkVisibility(polygon, currentPoint, ei->point(1)))
                        reachable.push_back(*ei);
                    ei--;
                }
                while(ei2!=polygon->edges_end()){//check for reachable edges from right side of preivous point
                    if(checkVisibility(polygon, currentPoint, ei2->point(0)) && checkVisibility(polygon, currentPoint, ei2->point(1)))
                        reachable.push_back(*ei2);
                    ei2++;
                }
            }
        }
        Segment newEdge = visibleEdgeSelector(currentPoint, reachable, edge);
        for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
            if(*vi==newEdge.point(0)){
                polygon->insert(vi, currentPoint);
                break;
            }
        }
    }
    
}

