#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/number_utils.h>
#include <CGAL/squared_distance_2.h>
#include "utility.hpp"
#include "edgeChange.hpp"
#include "mainFunctions.hpp"
#include <CGAL/Kd_tree.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Fuzzy_iso_box.h>
using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef K::Intersect_2 Intersect;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;
typedef CGAL::CartesianKernelFunctors::Intersect_2<K> Intersect;
typedef CGAL::Search_traits_2<K> Traits;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Fuzzy_iso_box<Traits>  Fuzzy_box;

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


