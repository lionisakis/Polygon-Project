#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/number_utils.h>
#include <CGAL/squared_distance_2.h>
#include "utility.hpp"

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef K::Intersect_2 Intersect;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;
typedef CGAL::CartesianKernelFunctors::Intersect_2<K> Intersect;





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
        cout <<"c h has " << KP.size() << endl;
        // find all red lines of KP
        vector<Segment> redLine;  
        // the previous is the last point so we can do a circle
        Point previous=KP.at(KP.size()-1);
        for(int i=0;i<KP.size();i++){
            // if positive then it is red
            if(checkRed(&KP,currentPoint,KP.at(i)) &&checkRed(&KP,currentPoint,previous)){    
                redLine.push_back(Segment(previous,KP.at(i)));
            }
            previous=KP.at(i);
        }
        vector<Segment> reachable;
        for(int j=0;j<redLine.size();j++){
            Point leftPoint=redLine.at(j).point(0);
            Point rightPoint=redLine.at(j).point(1);

            EdgeIterator positionStart,positionEnd;
            if(polygon->is_counterclockwise_oriented()==0){
                polygon->reverse_orientation();
            }
            // find the begin and end
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
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
                    if((checkVisibility(polygon, currentPoint, ei->point(0)))&&(checkVisibility(polygon, currentPoint, ei->point(1)))){  
                        reachable.push_back(*ei);
                    }
                }
                // [begin,...,end]
                for(EdgeIterator ei=polygon->edges_begin();ei<=positionEnd;ei++){
                    if((checkVisibility(polygon, currentPoint, ei->point(0)))&&(checkVisibility(polygon, currentPoint, ei->point(1)))){  
                        reachable.push_back(*ei);
                    }
                }
            }

        }  

        // select one edge
        Segment newEdge = visibleEdgeSelector(currentPoint, &reachable, edge,area);
        if(polygon->is_clockwise_oriented()==0){
            polygon->reverse_orientation();
        }

        // and add it in the polygon
        for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
            if(*vi==newEdge.point(0)){
                polygon->insert(vi, currentPoint);
                break;
            }
        }

    }
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }
}

void convexHull(Polygon* polygon, vector<Point>* points, int edge, double* area){
    //create KP
    vector<Point> KP;
    CGAL::convex_hull_2(points->begin(), points->end(), back_inserter(KP));
    for(int i=0; i<KP.size(); i++){
        polygon->push_back(KP.at(i));

    }
    vector<Point> remainingPoints;//store points that are not part of convex hull
    for(int k=0; k<points->size(); k++){
        int flag=1;
        for(int l=0; l<KP.size(); l++){
            if(points->at(k) == KP.at(l)){
                flag=0;
                break;
            }
        }
        if(flag){
            remainingPoints.push_back(points->at(k));
        }
    }

    //for each remaining point find its reachable edges and choose one to replace
    // for(int w=0; w<remainingPoints.size(); w++){
    //     Point currentPoint= remainingPoints.at(w);
    //     vector<Segment> reachable;
    //     for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
    //         if((checkVisibility(polygon, currentPoint, ei->point(0)))&&(checkVisibility(polygon, currentPoint, ei->point(1)))){  
    //             reachable.push_back(*ei);
    //         }
        
    //     }
    //     Segment newEdge = visibleEdgeSelector(currentPoint, &reachable, edge,area);
    //     for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
    //         if(*vi==newEdge.point(1)){
    //             polygon->insert(vi, currentPoint);
    //             break;
    //         }
    //     }
    // }
    int remaining = remainingPoints().size();

    while(remaining){
        vector<edgePointPair> pairs;
        //for every edge of polygon A find its closet point and create a pair that contains the edge the closest point and the area of the polygon that will be created
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            int min=Inf;
            int place;
            EdgeIterator ei2;
            for(int i=0; i<remaining; i++){
                int distance = CGAL::sqrt(CGAL::squared_distance(remainingPoints.at(i), ei));
                if(distance < min){
                    min=distance;
                    place=i;
                    ei2=ei;
                }
            }
            edgePointPair newPair(remainingPoints.at(i), *ei2);
            pairs.push_back(newPair);
        }

        //based on the type of edge selection  choose the according pair
        if(edge==1){
            int random = rand()%pairs.size();
            for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
                Point tmp = pairs.at(random).edge.point(1);
                if(*vi==tmp){
                    polygon->insert(vi, pairs.at(random).nearestPoint);
                    break;
                }
            }
        }
        else if(edge==2){
            int minArea=Inf;
            int choose;
            for(int j=0; j<pairs.size(); j++){
                if(pairs.at(j).area<minArea){
                    minArea=pairs.at(j).area;
                    choose = j;
                }
            }
            for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
                Point tmp = pairs.at(choose).edge.point(1);
                if(*vi==tmp){
                    polygon->insert(vi, pairs.at(choose).nearestPoint);
                    break;
                }
            }
        }
        else if(edge==3){
            int maxArea=0;
            int choose;
            for(int j=0; j<pairs.size(); j++){
                if(pairs.at(j).area>maxArea){
                    maxArea=pairs.at(j).area;
                    choose = j;
                }
            }
            for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
                Point tmp = pairs.at(choose).edge.point(1);
                if(*vi==tmp){
                    polygon->insert(vi, pairs.at(choose).nearestPoint);
                    break;
                }
            }

        }





        remaining--;
    }
}

