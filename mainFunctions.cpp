#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/number_utils.h>
#include <CGAL/squared_distance_2.h>
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




//flagSub=0: normal incremental, flagSub=1: incremental for subdivision
void incremental(Polygon* polygon,vector<Point>* points, int sorting, int edge,double* area, int flagSub=0){
    // make the triangle and short the points
    Point mostLeft, mostRight;
    vector<Point> LH;
    vector<Point> UH;
    if(!flagSub)
        coordinatesSorting(polygon,points,sorting,area);
    else{
        cout<<"!"<<endl;
        CGAL::lower_hull_points_2(points->begin(), points->end(), back_inserter(LH));
        CGAL::upper_hull_points_2(points->begin(), points->end(), back_inserter(UH));

        mostLeft = LH.at(0);
        mostRight = UH.at(0);
        polygon->push_back(points->at(0));
        polygon->push_back(points->at(1));
        cout<<"!!"<<endl;
        // so we are not collinear for KP reasons
        for(int i=2;i<points->size();i++){
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
    cout<<"-"<<endl;
    for (int i=3;i<points->size();i++){
        if(flagSub && (points->at(i) == mostLeft)){
            for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
                if(*vi==LH.at(1)){
                    polygon->insert(vi, points->at(i));
                    break;
                }
            }
            continue;
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
                    if(flagSub){
                        if(mostRight==ei->point(1))
                            continue;
                        if(mostLeft == ei->point(0))
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
                    if(flagSub){
                        if(mostRight==ei->point(1))
                            continue;
                        if(mostLeft == ei->point(0))
                            continue;
                    }
                    if((checkVisibility(polygon, currentPoint, ei->point(0)))&&(checkVisibility(polygon, currentPoint, ei->point(1)))){  
                        reachable.push_back(*ei);
                    }
                }
                // [begin,...,end]
                for(EdgeIterator ei=polygon->edges_begin();ei<=positionEnd;ei++){
                    if(flagSub){
                        if(mostRight==ei->point(1))
                            continue;
                        if(mostLeft == ei->point(0))
                            continue;
                    }
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

//flagSub=0: normal convex hull, flagSub=1: convex hull for subdivision
//if flagsub=1 then vector points is ordered by x increasing
void convexHull(Polygon* polygon, vector<Point>* points, int edge, double* area, int flagSub=0){
    
    //create convex hull and put it in the polygon
    vector<Point> KP;
    CGAL::convex_hull_2(points->begin(), points->end(), back_inserter(KP));
    for(int i=0; i<KP.size(); i++){
        polygon->push_back(KP.at(i));

    }
    *area = polygon->area();
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
    vector<Point> LH;
    CGAL::lower_hull_points_2(points->begin(), points->end(), back_inserter(LH));
    vector<Point> UH;
    CGAL::upper_hull_points_2(points->begin(), points->end(), back_inserter(UH));

    Point mostLeft, mostRight;
    if(flagSub){
        mostLeft = LH.at(0);
        mostRight = UH.at(0);
    }

    while(remainingPoints.size()!=0){
        vector<edgePointPair*> pairs;
        //for every edge of polygon A find its closet point and create a pair that contains the edge the closest point and the area of the polygon that will be created
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            double min=INFINITY;
            int place;
            for(int i=0; i<remainingPoints.size(); i++){
                // int distance = CGAL::sqrt(CGAL::squared_distance(remainingPoints.at(i), ei));
                double distance =triangularAreaCalculation(remainingPoints.at(i),ei->point(0),ei->point(1));
                if(distance < min){
                    if(flagSub==1){
                        if(polygon->is_counterclockwise_oriented()==1){
                            polygon->reverse_orientation();
                        }
                        if(ei->point(0) == mostRight || ei->point(1) == mostLeft){
                            continue;
                        }
                        if(polygon->is_counterclockwise_oriented()==0){
                            polygon->reverse_orientation();
                        }
                            
                    }
                    else if(flagSub==2){
                        if(polygon->is_counterclockwise_oriented()==1){
                            polygon->reverse_orientation();
                        }
                        if(ei->point(1) == mostRight || ei->point(0) == mostLeft){
                            continue;
                        }
                        if(polygon->is_counterclockwise_oriented()==0){
                            polygon->reverse_orientation();
                        }
                            
                    }
                    if((checkVisibility(polygon, remainingPoints.at(i), ei->point(0)))&&(checkVisibility(polygon,remainingPoints.at(i), ei->point(1)))){
                        min=distance;
                        place=i;
                    }
                }
            }
            if (min!=INFINITY){
                edgePointPair* newPair = new edgePointPair(place,remainingPoints.at(place),*ei);
                pairs.push_back(newPair);
            }
        }
        if(polygon->is_counterclockwise_oriented()==0){
            polygon->reverse_orientation();
        }
        //the program exits if no pair can be found, as was requested to do in one of the threads in eclass
        if(pairs.size() == 0){
            cout << "reached deadend --- about to exit"<< endl;;
            exit(1);
        }
        int choose;
        //based on the type of edge selection  choose the according pair
        if(edge==1){
            choose = rand()%pairs.size();
        }
        else if(edge==3){
            double minArea=INFINITY;
            for(int j=0; j<pairs.size(); j++){
                if(pairs.at(j)->getArea()<minArea){
                    minArea=pairs.at(j)->getArea();
                    choose = j;
                }
            }
        }
        else if(edge==2){
            int maxArea=0;
            for(int j=0; j<pairs.size(); j++){
                if(pairs.at(j)->getArea()>maxArea){
                    maxArea=pairs.at(j)->getArea();
                    choose = j;
                }
            }
        }
        
        for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end(); vi++){
            Point tmp = pairs.at(choose)->getSegment().point(1);
            if(*vi==tmp){
                polygon->insert(vi, pairs.at(choose)->getPoint());
                *area = *area - pairs.at(choose)->getArea();
                break;
            }
        }
        remainingPoints.erase(remainingPoints.begin()+pairs.at(choose)->getPosition());

        for (int i=0;i<pairs.size();i--){
            delete pairs.at(i);
        }        
    }
    cout <<"most left = " << mostLeft << endl;
    cout <<"most right = " << mostRight << endl;
}

