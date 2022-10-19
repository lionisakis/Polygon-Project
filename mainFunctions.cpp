#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include "utility.hpp"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
// typedef CGAL::convex_hull_2 convexHull;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;
typedef CGAL::CartesianKernelFunctors::Intersect_2<K> Intersect;

int checkVisibility(Segment segment, Point newPoint, Point checkPoint){
    Segment rayCast(newPoint, checkPoint);

    if(intersection(rayCast, segment)){
        //cout<<"0"<<endl;
        return 0;
    }

    //cout<<"1"<<endl;
    return 1;
}


void incremental(Polygon* polygon,vector<Point>* points, int sorting, int edge,double* area){
    
    // make the triangle and short the points
    coordinatesSorting(polygon,points,sorting,area);
    for (int i=3;i<points->size();i++){
        // find the current Point we want to insert
        Point currentPoint= points->at(i);

        // find KP 
        vector<Point> KP;
        const Polygon::Vertices& range = polygon->vertices();
        CGAL::convex_hull_2(range.begin(), range.end(), back_inserter(KP));

        // find all red lines of KP
        vector<Segment> redLine;  
        // the previous is the last point so we can do a circle
        Point previous=KP.at(KP.size()-1);
        for(int i=0;i<KP.size();i++){
            // if positive then it is red
            if(isItRed(&KP,previous,KP.at(i),currentPoint)){
                redLine.push_back(Segment(previous,KP.at(i)));
            }
            previous=KP.at(i);
        }
        cout<<"!!"<<endl;

        vector<Segment> reachable;
        for(int i=0;i<redLine.size();i++){
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                // BUG HERE
                if(checkEdgeInsideRedLine(*ei,redLine.at(i),sorting)==0){
                    cout<<"Continue:"<<*ei<<" redline"<<redLine.at(i)<<endl;
                    continue;
                }
                cout<<*ei<<" "<<redLine.at(i)<<endl;

                // we are in red line area and start checking
                if(isItReachable(polygon,ei->point(0),ei->point(1),currentPoint)){
                    reachable.push_back(*ei);
                }
            }
        }    

        Segment newEdge = visibleEdgeSelector(currentPoint, &reachable, edge,area);
        //cout << "adding " << newEdge << endl;
        for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
            if(*vi==newEdge.point(1)){
                polygon->insert(vi, currentPoint);
                break;
            }
        }
    }
}

