#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "edgeChange.hpp"
#include "genericUtil.hpp"
#include <math.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;


using namespace std;

//computes the area of a triangle
double triangularAreaCalculation(Point point0, Point point1, Point point2){
    
    // det|a b; c d|
    double a = point1.x() - point0.x();
    double b = point1.y() - point0.y();
    double c = point2.x() - point0.x();
    double d = point2.y() - point0.y();

    double det=a*d-b*c;
    if (det<0){
        det*=(-1);
    }
    return det/2;
}


//sort points based on type and then create a triangle from the first 3 points
void coordinatesSorting(Polygon* polygon,vector<Point>* points,int type,double* area){
    for (int i=0;i<points->size();i++){
        for(int j=i+1;j<points->size();j++){
            swap(&points->at(i),&points->at(j),type);
        }
    }    
    
    polygon->push_back(points->at(0));
    polygon->push_back(points->at(1));
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

//choose from a list of various visible edges based on type=
//1:choose randomly
//2: choose the one that minimizes area
//3: chooses the onat that maximizes area
Segment visibleEdgeSelector(Point newPoint,vector<Segment>* vector, int type,double* area){
    Segment theValue;
    if (type==1){
        unsigned int tmp = (unsigned) time(NULL);
        srand(tmp); 
        int random= rand()%vector->size();
        theValue=vector->at(random);
        *area+=triangularAreaCalculation(newPoint,theValue.point(0),theValue.point(1));
    }
    else if (type==2){
        // initialize the first min
        Segment edge=vector->at(0);
        double min= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
        Segment minEdge=edge;

        // find the new min
        for (int i=1;i<vector->size();i++){
            edge=vector->at(i);
            double area= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
            if (area<min){
                min=area;
                minEdge=edge;
            }
        }
        *area+=min;
        theValue=minEdge;
    }
    else{
        // initialize the first max
        Segment edge=vector->at(0);

        double max= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
        Segment maxEdge=edge;

        // find the new max
        for (int i=1;i<vector->size();i++){
            edge=vector->at(i);

            double area= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
            if (area>max){
                max=area;
                maxEdge=edge;
            }
        }
        *area+=max;
        theValue=maxEdge;
    }
    return theValue;
}

//check if edge is visible
int checkVisibility(Polygon* polygon, Point newPoint, Point checkPoint){
    Segment rayCast(newPoint, checkPoint);
    for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
        if(ei->point(0) == newPoint || ei->point(1)==newPoint){
            continue;
        }
        if(ei->point(0) != checkPoint && ei->point(1)!=checkPoint){
            if(intersection(rayCast, *ei)){
                return 0;
            }
        }
    }
    return 1;
}

// check if the KP point is red or not
int checkRed(vector<Point>* KP, Point newPoint, Point checkPoint){
    Segment rayCast(newPoint, checkPoint);
    Point previous=KP->at(KP->size()-1);
    for(int u=0; u<KP->size(); u++){
        if(KP->at(u) != checkPoint && previous!=checkPoint){
            Segment rayCast2(KP->at(u), previous);
            if(intersection(rayCast, rayCast2)){
                return 0;
            }
            }
        previous=KP->at(u);
    }
    return 1;
}

//it has to be clockwise!!!
//calculate the area of the polygon that will be created if we implement the change
//substract from it the initial area and return the product
double calculateNewArea(Polygon* polygon, Segment edge, Point left, Point right, vector<Point>* path){
    double total=0.0;
    int length = path->size(); //length of path
    //create a temporary polygon with its edges and compute its area
    Polygon temporary(*polygon);

    Point whereToInsert=edge.point(1);
    VertexIterator tempLeft,tempRight,insert;
    for (VertexIterator vi=temporary.vertices_begin();vi!=temporary.end();vi++){
        if(*vi==left){
            tempLeft=vi;
            break;
        }
    }
    vector<Point> points;
    VertexIterator vi=tempLeft;
    while(*vi!=right){
        
        Point temp=*vi;
        points.push_back(temp);
        temporary.erase(vi);
        if(vi==temporary.vertices_end())
            vi=temporary.vertices_begin();
    }
    points.push_back(right);
    temporary.erase(vi);
    int count=0;
    Point prev;
    for (VertexIterator vi=temporary.vertices_begin();vi!=temporary.vertices_end();vi++){
        if (*vi==whereToInsert){
            int i;
            temporary.insert(vi,points.at(0));
            count++;
            prev = points.at(0);
            break;
        }
    }
    while(count<points.size()){
        VertexIterator tmp;
        for (VertexIterator vi=temporary.vertices_begin();vi!=temporary.end();vi++){
            if(*vi==prev){
                tmp=vi;
                break;
            }
        }
        temporary.insert(tmp,points.at(count));
        prev = points.at(count);
        count++;
    }
    total = abs(temporary.area()) - abs(polygon->area());
    return total;
}

//implement that change that we chose, move path V between u1u2
void changeEdge(Polygon* polygon,EdgeChange* edge, int total){
    Point left=edge->getLeft();
    Point right=edge->getRight();
    Point whereToInsert=edge->getSegment().point(1);
    VertexIterator tempLeft,tempRight,insert;
    for (VertexIterator vi=polygon->vertices_begin();vi!=polygon->end();vi++){
        if(*vi==left){
            tempLeft=vi;
            break;
        }
    }
    vector<Point> points;
    VertexIterator vi=tempLeft;
    while(*vi!=right){
        
        Point temp=*vi;
        points.push_back(temp);
        polygon->erase(vi);
        if(vi==polygon->vertices_end())
            vi=polygon->vertices_begin();
    }
    points.push_back(right);
    polygon->erase(vi);
    int count=0;
    Point prev;
    for (VertexIterator vi=polygon->vertices_begin();vi!=polygon->vertices_end();vi++){
        if (*vi==whereToInsert){
            int i;
            polygon->insert(vi,points.at(0));
            count++;
            prev = points.at(0);
            break;
        }
    }
    while(count<points.size()){
        VertexIterator tmp;
        for (VertexIterator vi=polygon->vertices_begin();vi!=polygon->end();vi++){
            if(*vi==prev){
                tmp=vi;
                break;
            }
        }
        polygon->insert(tmp,points.at(count));
        prev = points.at(count);
        count++;
    }
}

//returns 1 when the path does not pass the validity check and 0 when it passes the validity check
int checkPath(Polygon* polygon,VertexIterator viPathFirst,VertexIterator viPathLast,EdgeIterator ei2){
    // and not in our edge

    if(ei2->point(0)==*viPathLast||ei2->point(1)==*viPathLast){
        return 1;
    }
    // check visibility that the last point is visible
    if ((checkVisibility(polygon,*viPathLast,ei2->point(0))==0)||(checkVisibility(polygon,*viPathLast,ei2->point(1)))==0)
        return 1;
    // check the change does not change our simplicity
    Segment segLeft=Segment(ei2->point(1),*viPathFirst);
    Segment segRight=Segment(*viPathLast,ei2->point(0));
    Segment newEi;    

    if(viPathLast==polygon->vertices_end()-1){
        newEi=Segment(*(viPathFirst-1),*(polygon->vertices_begin()));  
    }
    else if (viPathFirst==polygon->vertices_begin()){
        newEi=Segment(*(polygon->vertices_end()-1),*(viPathLast+1));                                      
    }
    else{
        newEi=Segment(*(viPathFirst-1),*(viPathLast+1));                             
    }
    if(intersection(newEi,segLeft)||intersection(newEi,segRight))
        return 1;
    //check if the new edge newEi intersects with the other edges of the polygon
    for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
        if(ei->point(0)==newEi.point(0)||ei->point(1)==newEi.point(0))
            continue;
        if(ei->point(0)==newEi.point(1)||ei->point(1)==newEi.point(1))
            continue;
        if((*ei == Segment(ei2->point(0), *viPathFirst)) || (*ei == Segment(*viPathLast, ei->point(1))))
            continue;
        if(intersection(newEi,*ei))
            return 1;
    }
    return 0;
}


