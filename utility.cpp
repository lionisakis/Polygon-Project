#include <iostream>
#include <cstdlib>
#include <list>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;

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
    if (type==0){
        srand((unsigned) time(NULL)); 
        int random= rand()%vector.size();
        theValue=vector.at(random);
    }
    else if (type==1){
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


// int main(void){

//     vector<Segment> vector;
//     vector.push_back(Segment(Point(5,5),Point(5,10)));
//     vector.push_back(Segment(Point(5,10),Point(10,5)));
//     vector.push_back(Segment(Point(10,5),Point(5,5)));
//     Segment what=visibleEdgeSelector(Point(0,0),vector,2);
//     cout<<what<<"\n";
// }