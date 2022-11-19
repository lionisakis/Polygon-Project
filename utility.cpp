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

int calculateNewArea(Polygon* polygon, Segment edge, Point left, Point right, vector<Point>* path){
    int total=0;
    Point left2; //neighbour of left point 
    Point right2; //neighbour of right point 
    int length = path.size(); //length of path
    for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
        if(ei->point(0) == right)
            right2 = ei->point(1);

        if(ei->point(1) == left )
            left2 = ei->point(0);
    }
    //compute the area that we have to add
    total+=triangularAreaCalculation(path->at(0), left2, right2);
    for(int i=1; i<length; i++){
        total+=triangularAreaCalculation(path->at(i), path.at(i-1), right2);
    }

    //compute the area that we will lose
    total-=triangularAreaCalculation(path->at(0), edge.point(0), edge.point(1));
    for(int i=1; i<length; i++){
        total-=triangularAreaCalculation(path->at(i), path.at(i-1), edge.point(1));
    }
    return total;
}