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

Segment visibleEdgeSelector(Point newPoint,vector<Segment> vector, int type){
    Segment theValue;
    if (type==1){
        srand((unsigned) time(NULL)); 
        int random= rand()%vector.size();
        theValue=vector.at(random);
    }
    else if (type==2){
        // initialize the first max
        Segment edge=vector.at(0);
        double max= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
        Segment maxEdge=edge;

        // find the new max
        for (int i=1;i<vector.size();i++){
            edge=vector.at(i);
            double area= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
            if (area>max){
                max=area;
                maxEdge=edge;
            }
        }
        theValue=maxEdge;
    }
    else{
        // initialize the first min
        Segment edge=vector.at(0);
        double min= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
        Segment minEdge=edge;

        // find the new min
        for (int i=1;i<vector.size();i++){
            edge=vector.at(i);
            double area= triangularAreaCalculation(newPoint,edge.point(0),edge.point(1));
            if (area<min){
                min=area;
                minEdge=edge;
            }
        }
        theValue=minEdge;
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
void coordinatesSorting(Polygon* polygon,vector<Point>* points,int type){
    for (int i=0;i<points->size();i++){
        for(int j=i+1;j<points->size();j++){
            swap(&points->at(i),&points->at(j),type);
        }
    }    
    polygon->push_back(points->at(0));
    polygon->push_back(points->at(1));
    polygon->push_back(points->at(2));
}

// int main(void){

//     vector<Point> vector;
//     vector.push_back(Point(1,1));
//     vector.push_back(Point(2,2));
//     vector.push_back(Point(2,3));
//     vector.push_back(Point(3,3));
//     Polygon polygon;
//     coordinatesSorting(&polygon,&vector,4);
//     for(const Segment& e  : polygon.edges()){
//         std::cout << e << std::endl;
//     }

//     for (int i=0;i<vector.size();i++){
//         cout<<vector.at(i)<<"\n";
//     }
// }
//1: x decreasing 
//2: x increasing 
//3: y decreasing
//4: y increasing