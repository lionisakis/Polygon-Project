#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "utility.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;


class EdgeChange{
    private:
        Segment segment;
        Point right;
        Point left;
        int area;
    public:
        EdgeChange(Point ,Point, Segment, int);//left right edge area
        const Point getLeft();
        const Point getRight();
        const Segment getSegment();
        const double getArea();
};