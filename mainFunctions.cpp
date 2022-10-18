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
    //cout<<"raycast: "<<raycast<<endl;
    for(EdgeIterator ei=p->edges_begin();ei!=p->edges_end();ei++){
        // if the checkpoint is in the segment then do not consider it
        // as it is the reachable edge
        if (ei->point(0)==checkPoint || ei->point(1)==checkPoint)
            continue;

        //cout<<"newPoint: "<<raycast<<endl;
        if(intersection(raycast, *ei)){
            //cout<<"0"<<endl;
            return 0;

        }
    }
    //cout<<"1"<<endl;
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
                //reachable.push_back(*ei);
                //cout << "first  " << *ei << "  prev = "<< previousPoint << " curr = " << currentPoint << endl;
                // ei2-1 so we do not start from the same point
                EdgeIterator ei2=ei;
                EdgeIterator ei3=ei+1;
                // check until the first edge is not visible
                while(ei2!=polygon->edges_begin()){//check for reachable edges from left side of preivous point
                    int flag1=1;
                    if(checkVisibility(polygon, currentPoint, ei2->point(0)) && checkVisibility(polygon, currentPoint, ei2->point(1))){
                        //cout << "found reachable 1  " << *ei2 << endl;
                        flag1=0;
                        reachable.push_back(*ei2);
                    }
                    else
                        if(!flag1)//flag1=0: we have found reachable edges can stop searching cause we stuble upon blue line, flag1=1: no reachable edges found must continue searching even after blue line
                            break;
                    //cout << "before " << *ei2 << endl;
                    ei2--;
                    // cout << "after " << *ei2 << endl;
                    // cout <<"ready for loop"<< endl;
                }
                //cout <<"11111"<< endl;
                // check if the first edges is visibly as we cannot check it in the while
                if (ei2==polygon->edges_begin()){
                    if(checkVisibility(polygon, currentPoint, ei2->point(0)) && checkVisibility(polygon, currentPoint, ei2->point(1))){
                        //cout << "found reachable 2   " << *ei2 << endl;
                        reachable.push_back(*ei2);
                    }
                }
                //cout << "22222" << endl;
                while(ei3!=polygon->edges_end()){//check for reachable edges from right side of preivous point
                    int flag2=1;
                    if(checkVisibility(polygon, currentPoint, ei3->point(0)) && checkVisibility(polygon, currentPoint, ei3->point(1))){
                        //cout << "found reachable 3   " << *ei3 << endl;
                        flag2=0;
                        reachable.push_back(*ei3);
                    }
                    else
                        if(!flag2)//flag2=0: we have found reachable edges can stop searching cause we stuble upon blue line, flag2=1: no reachable edges found must continue searching even after blue line
                            break;
                    ei3++;
                }
                //cout <<"33333"<<endl;

                

            }
        }
        Segment newEdge = visibleEdgeSelector(currentPoint, &reachable, edge);
        //cout << "adding " << newEdge << endl;
        for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
            if(*vi==newEdge.point(1)){
                polygon->insert(vi, currentPoint);
                break;
            }
        }
        // cout << "new polygon" << endl;
        // for (EdgeIterator ei5 = polygon->edges_begin(); ei5 != polygon->edges_end(); ++ei5)
        //     cout << *ei5 << endl;
        // cout << "simple = " << polygon->is_simple() << endl;
        // cout << "end" << endl;

    }
    
}

