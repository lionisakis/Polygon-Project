#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/number_utils.h>
#include <CGAL/squared_distance_2.h>
#include "geoUtil.hpp"
#include "genericUtil.hpp"
#include "edgePointPair.hpp"
#include "timeManager.hpp"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef K::Intersect_2 Intersect;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;
typedef CGAL::CartesianKernelFunctors::Intersect_2<K> Intersect;



//flagSub=0: normal incremental, flagSub=1: incremental for subdivision
int incremental(Polygon* polygon,vector<Point>* points, int sorting, int edge,double* area, int flagSub=0,Segment* leftRight=NULL, int lastSet=0){
    if(checkCutOf())
        return -10;
    // make the triangle and short the points
    if(!flagSub){
        coordinatesSorting(polygon,points,sorting,area);
    }
    else{
        // it is clockwise so r->q
        Point r=leftRight[0].point(1);
        polygon->push_back(r);
        polygon->push_back(points->at(0));
        
        // find posLeft
        int posLeft;
        for(int i=0;i<points->size();i++){
            if(points->at(i)==r){
                posLeft=i;
                break;
            }
        }

        
        // // swap the r to be the first point
        Point tmp = points->at(1);
        points->at(1) = points->at(posLeft); 
        points->at(posLeft) = tmp;

        for (int i=2; i <=posLeft; i++){
            for(int j=i;j>2;j--){
                if(points->at(j-1).x()>points->at(j).x()){
                    Point temp=points->at(j);
                    points->at(j)=points->at(j-1);
                    points->at(j-1)=temp;
                }
            }
        }
        // so we are not collinear for KP reasons
        for(int i=2;i<points->size();i++){
            if(checkCutOf())
                return -10;
            Point third=points->at(i);
            // if they are not collinear
            if (! CGAL::collinear(points->at(0),points->at(1),third)){
                polygon->push_back(third);
                // swap go to the 3rd position of points so we have almost sorted points
                for(int j=i;j>2;j--){
                    Point temp=points->at(j);
                    points->at(j)=points->at(j-1);
                    points->at(j-1)=temp;
                }
                break;
            }
        }
        *area=triangularAreaCalculation(points->at(0),points->at(1),points->at(2));
    }
    for (int i=3;i<points->size();i++){
        if(checkCutOf())
            return -10;
        if(flagSub && i==points->size()-1 && lastSet==0){
            Point p=leftRight[1].point(0);
            Point q=leftRight[1].point(1);
            EdgeIterator ei;
            for (ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                if(ei->point(1)==p)
                    break;
            }
            vector<Segment> reachable;
            if((checkVisibility(polygon, q, ei->point(0)))&&(checkVisibility(polygon, q, ei->point(1)))){  
                reachable.push_back(*ei);
            }

            if(reachable.size()==0){
                if(flagSub)
                    return 1;
                else
                    exit(1);
            }
            // select one edge
            Segment newEdge = visibleEdgeSelector(q, &reachable, edge,area);
            if(polygon->is_clockwise_oriented()==0){
                polygon->reverse_orientation();
            }

            // and add it in the polygon
            for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
                if(checkCutOf())
                    return -10;
                if(*vi==newEdge.point(1)){
                    polygon->insert(vi, q);
                    break;
                }
            }
            int flagl =0 , flagr =0;
            if(polygon->is_counterclockwise_oriented()==0){
                polygon->reverse_orientation();
            }

            for(EdgeIterator ei = polygon->edges_begin(); ei!=polygon->edges_end(); ei++){
                if(checkCutOf())
                    return -10;
                if(*ei == leftRight[0]){
                    flagl=1;
                }
                else if(*ei == leftRight[1]){
                    flagr = 1;
                }
            }
            if(flagl == 0 || flagr == 0){
                //cout <<"flagl = " << flagl << " flagr = " << flagr << endl;
                return 1;
            }
            if(polygon->is_simple() == 0)
                return 1;
            return 0;
        }
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
            if(checkCutOf())
                return -10;
            // if positive then it is red
            if(checkRed(&KP,currentPoint,KP.at(i)) &&checkRed(&KP,currentPoint,previous)){    
                redLine.push_back(Segment(previous,KP.at(i)));
            }
            previous=KP.at(i);
        }
        if(flagSub && redLine.size()==0){
            return 1;
        }
        vector<Segment> reachable;
        for(int j=0;j<redLine.size();j++){
            if(checkCutOf())
                return -10;
            Point leftPoint=redLine.at(j).point(0);
            Point rightPoint=redLine.at(j).point(1);

            EdgeIterator positionStart,positionEnd;
            if(polygon->is_counterclockwise_oriented()==0){
                polygon->reverse_orientation();
            }
            // find the begin and end
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                if(checkCutOf())
                    return -10;
                if (ei->point(0)==leftPoint){
                    positionStart=ei;
                }
                if(ei->point(1)==rightPoint){
                    positionEnd=ei;                    
                }
            }
            // the red line is normal and the edges have the form of:
            // [begin,...,start,...,end,...,finish]
            // so we need only [start,..,end]
            if(positionStart<=positionEnd){    
                // the edges between the start and end check if they are visible
                for(EdgeIterator ei=positionStart;ei<=positionEnd;ei++){
                    if(checkCutOf())
                        return -10;
                    if(flagSub){
                        if(leftRight[0] == *ei)
                            continue;
                        else if(leftRight[1] == *ei)
                            continue;
                    }

                    if((checkVisibility(polygon, currentPoint, ei->point(0)))&&(checkVisibility(polygon, currentPoint, ei->point(1)))){  
                        reachable.push_back(*ei);
                    }
                }
            }
            // the red line loops back and it is the form of 
            // [begin,...,end,...,start,...finish]
            // so we need [start,...,finish] and [begin,...,end]
            else{
                // [start,...,finish] 
                for(EdgeIterator ei=positionStart;ei!=polygon->edges_end();ei++){
                    if(checkCutOf())
                        return -10;
                    if(flagSub){
                        if(leftRight[0] == *ei){
                            continue;
                        }
                        else if(leftRight[1] == *ei){
                            continue;
                        }
                    }
                    if((checkVisibility(polygon, currentPoint, ei->point(0)))&&(checkVisibility(polygon, currentPoint, ei->point(1)))){  
                        reachable.push_back(*ei);
                    }
                }
                // [begin,...,end]
                for(EdgeIterator ei=polygon->edges_begin();ei<=positionEnd;ei++){
                    if(checkCutOf())
                        return -10;
                    if(flagSub){
                        if(leftRight[0] == *ei){
                            continue;
                        }
                        else if(leftRight[1] == *ei){
                            continue;
                        }
                    }
                    
                    if((checkVisibility(polygon, currentPoint, ei->point(0)))&&(checkVisibility(polygon, currentPoint, ei->point(1)))){  
                        reachable.push_back(*ei);
                    }
                }
            }

        }  
        if(flagSub && reachable.size()==0){
            return 1;
        }
        // select one edge
        Segment newEdge = visibleEdgeSelector(currentPoint, &reachable, edge,area);
        if(polygon->is_clockwise_oriented()==0){
            polygon->reverse_orientation();
        }

        // and add it in the polygon
        for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
            if(checkCutOf())
                return -10;
            if(*vi==newEdge.point(0)){
                polygon->insert(vi, currentPoint);
                break;
            }
        }

    }
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }
    return 0;
}
