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

//typeOfOptimization=1: max area, typeOfOptimization=2: min area
void localSearch(Polygon* polygon, int typeOfOptimization, int threshold, int L){
    int DA=0;
    int flag=1;
    int initialArea = polygon->area();
    do{
        vector<edgeChange*> changes;
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi){
                for (VertexIterator vi2 = vi; vi2 != p.vertices_end(); ++vi){

                }
            }
        }





    }while(DA>=threshold);

}