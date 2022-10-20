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


using namespace std;
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

Segment visibleEdgeSelector(Point newPoint,vector<Segment>* vector, int type,double* area){
    Segment theValue;
    if (type==1){
        srand((unsigned) time(NULL)); 
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

//1: x decreasing 
//2: x increasing 
//3: y decreasing
//4: y increasing
void swap(Point* a, Point* b,int type){
    // x decreasing
    if (type==1){
        if(a->x()<b->x()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
    // x increasing
    else if (type==2){
        if(a->x()>b->x()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
    // y decreasing
    else if(type==3){
        if(a->y()<b->y()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
    // y increasing
    else{
        if(a->y()>b->y()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
}
void coordinatesSorting(Polygon* polygon,vector<Point>* points,int type,double* area){
    for (int i=0;i<points->size();i++){
        for(int j=i+1;j<points->size();j++){
            swap(&points->at(i),&points->at(j),type);
        }
    }    
    polygon->push_back(points->at(0));
    polygon->push_back(points->at(1));
    polygon->push_back(points->at(2));
    *area=triangularAreaCalculation(points->at(0),points->at(1),points->at(2));
}

// check if the KP point is red or not
int isItRed(vector<Point>* KP,Point a,Point b,Point p){
    int positive=a.x()*b.y()*1+a.y()*1*p.x()+1*b.x()*p.y();
    int negative=p.x()*b.y()*1+p.y()*1*a.x()+1*b.x()*a.y();
    int det1=positive-negative;
    
    // for all KP point (so random KP point)
    // check if it is blue or red
    for(int i=0;i<KP->size();i++){
        Point q=KP->at(i);
        if(q==a||q==b)
            continue;
        positive=a.x()*b.y()*1+a.y()*1*q.x()+1*b.x()*q.y();
        negative=q.x()*b.y()*1+q.y()*1*a.x()+1*b.x()*a.y();
        int det2=positive-negative;
        // I am blue
        if ((det2>=0 && det1>=0) || (det2<0 && det1<0))
            return 0;
    }

    // else I am red
    return 1;
}

// check if the polygon point is red or not
int isItReachable(Polygon* polygon,Point a,Point b,Point p){
    int positive=(a.x()*b.y()*1)+(a.y()*1*p.x())+(1*b.x()*p.y());
    int negative=(p.x()*b.y()*1)+(p.y()*1*a.x())+(1*b.x()*a.y());
    int det1=positive-negative;
    
    // for all polygon points 
    // check if it is reachable or not
    for(VertexIterator vi=polygon->vertices_begin();vi!=polygon->vertices_end();vi++){
        Point q=*vi;
        if(q==a||q==b)
            continue;
        positive=(a.x()*b.y()*1)+(a.y()*1*q.x())+(1*b.x()*q.y());
        negative=(q.x()*b.y()*1)+(q.y()*1*a.x())+(1*b.x()*a.y());
        int det2=positive-negative;
        // I am not reachable
        if ((det2>=0 && det1>=0) || (det2<0 && det1<0)){
            return 0;
        }
    }

    // I am reachable
    return 1;
}

int checkEdgeInsideRedLine(Segment polygonSegment,Segment redline,int sorting){
    Point a=polygonSegment.point(0);
    Point b=polygonSegment.point(1);
    Point redline1=redline.point(0);
    Point redline2=redline.point(1);
    int lower=0;
    int upper=0;
    // sort by x
    if(sorting<=2){        
        if (redline1.y()<redline2.y()){
            lower=redline1.y();
            upper=redline2.y();
        }
        else{
            lower=redline2.y();
            upper=redline1.y();
        }
        if(lower<=a.y()&& a.y()<=upper && lower<=b.y()&& b.y()<=upper)
            return 1;

    }
    // sort by y
    else{
        if (redline1.x()<redline2.x()){
            lower=redline1.x();
            upper=redline2.x();
        }
        else{
            lower=redline2.x();
            upper=redline1.x();
        }
        if(a.x()>=lower&& a.x()<=upper && b.x()>=lower&& b.x()<=upper)
            return 1;
    }
    return 0;
}