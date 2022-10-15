#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;

double triangularAreaCalculation(Point point0, Point point1, Point point2){
    
    // det|a b; c d|
    double a = point1.x() - point0.x();
    double b = point1.y() - point0.y();
    double c = point2.x() - point0.x();
    double d = point2.x() - point0.y();

    double det=a*d-b*c;
    
    return det/2;
}

int main(void){
    printf("%f\n",triangularAreaCalculation(Point(0,0),Point(0,1),Point(1,1)));
}