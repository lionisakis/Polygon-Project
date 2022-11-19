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
    // change to clockwise 
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }

    int DA=0;
    int flag=1;
    int initialArea = abs(polygon->area());
    do{
        vector<EdgeChange*> changes;
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
                
                // check visibility
                if (checkVisibility(polygon,*vi,ei->point(0))&&checkVisibility(polygon,*vi,ei->point(1)))
                    continue;

                vector<Point> path;
                path.push_back(*vi);
                double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                changes.push_back(newChange);

                // until we have gone out of bounds do this
                for (VertexIterator vi2 = vi; vi2 != polygon->vertices_end(); ++vi2){
                    path.push_back(*vi2);
                    // check visibility
                    if (checkVisibility(polygon,*vi,ei->point(0))&&checkVisibility(polygon,*vi,ei->point(1)))
                        continue;

                    // check if the path is ok with the size
                    if(path.size()>L){
                        break;
                    }

                    double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                    EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                    changes.push_back(newChange);
                }

                // this do when we have to start from the starting vertex again
                for (VertexIterator vi2 = polygon->vertices_begin(); vi2 != vi; ++vi2){
                    path.push_back(*vi2);

                    // check visibility
                    if (checkVisibility(polygon,*vi,ei->point(0))&&checkVisibility(polygon,*vi,ei->point(1)))
                        continue;

                    // check if the path is ok with the size
                    if(path.size()>L){
                        break;
                    }

                    double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                    EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                    changes.push_back(newChange);
                }
            }
        }
        // pick the correct change
        int temp=changes.at(0)->getArea();
        EdgeChange* theChange=changes.at(0);
        
        for(int i=1;i<changes.size();i++){
            // max
            if (typeOfOptimization==1 && temp<changes.at(i)->getArea()){
                temp=changes.at(i)->getArea();
                theChange=changes.at(i);
            }
            // min
            else if (typeOfOptimization==2 && temp>changes.at(i)->getArea()){
                temp=changes.at(i)->getArea();
                theChange=changes.at(i);
            }
        }


        // remove edge and change

        // DA= abs(new area-old area) 

    }while(DA>=threshold);

}