#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include "genericUtil.hpp"
#include "geoUtil.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;

class edgePointPair{
    private:
        int position;
        Point point;
        Segment segment;
        double area;

    public:
        edgePointPair(int ,Point, Segment);
        const int getPosition();
        const double getArea();
        const Point getPoint();
        const Segment getSegment();
};