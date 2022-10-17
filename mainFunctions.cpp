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
    cout<<"raycast: "<<raycast<<endl;
    for(EdgeIterator ei=p->edges_begin();ei!=p->edges_end();ei++){
        // if the checkpoint is in the segment then do not consider it
        // as it is the reachable edge
        if (ei->point(0)==checkPoint || ei->point(1)==checkPoint)
            continue;

        cout<<"newPoint: "<<raycast<<endl;
        if(intersection(raycast, *ei)){
            cout<<"0"<<endl;
            return 0;

        }
    }
    cout<<"1"<<endl;
    return 1;
}


void incremental(Polygon* polygon,vector<Point>* points, int sorting, int edge){
    
    // make the triangle and short the points
    coordinatesSorting(polygon,points,sorting);
    for (int i=3;i<points->size();i++){
        // initialize the vector with size for complexity 
        vector<Segment> reachable;

        Point currentPoint= points->at(i);
        Point previousPoint=points->at(i-1);
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            if(ei->point(1) == previousPoint){//we found the left red line 
                reachable.push_back(*ei);
                // ei2-1 so we do not start from the same point
                EdgeIterator ei2=ei-1;
                EdgeIterator ei3=ei;
                // check until the first edge is not visible
                while(ei2!=polygon->edges_begin()){//check for reachable edges from left side of preivous point
                    if(checkVisibility(polygon, currentPoint, ei2->point(0)) && checkVisibility(polygon, currentPoint, ei2->point(1)))
                        reachable.push_back(*ei2);
                    else
                        break;
                    ei2--;
                }
                // check if the first edges is visibly as we cannot check it in the while
                if (ei2==polygon->edges_begin())
                    if(checkVisibility(polygon, currentPoint, ei2->point(0)) && checkVisibility(polygon, currentPoint, ei2->point(1))){
                        reachable.push_back(*ei2);
                    }
                
                while(ei3!=polygon->edges_end()){//check for reachable edges from right side of preivous point
                    if(checkVisibility(polygon, currentPoint, ei3->point(0)) && checkVisibility(polygon, currentPoint, ei3->point(1)))
                        reachable.push_back(*ei3);
                    else
                        break;
                    ei3++;
                }

                

            }
        }
        Segment newEdge = visibleEdgeSelector(currentPoint, &reachable, edge);
        for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
            if(*vi==newEdge.point(1)){
                polygon->insert(vi, currentPoint);
                break;
            }
        }
    }
    
}

