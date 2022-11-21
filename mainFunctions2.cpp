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

int checkPath(Polygon* polygon,VertexIterator viPathFirst,VertexIterator viPathLast,EdgeIterator ei){
    // check visibility that the last point is visible
    if ((checkVisibility(polygon,*viPathLast,ei->point(0))==0)||(checkVisibility(polygon,*viPathLast,ei->point(1)))==0)
        return 1;
    // and not in our edge
    if(ei->point(0)==*viPathLast||ei->point(1)==*viPathLast){
        return 1;
    }
    // check the change does not change our simplicity
    Segment segLeft=Segment(ei->point(0),*viPathFirst);
    Segment segRight=Segment(*viPathLast,ei->point(1));
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
    return 0;
}
//typeOfOptimization=1: max area, typeOfOptimization=2: min area
void localSearch(Polygon* polygon, int typeOfOptimization, int threshold, int L, int* finalArea){
    // change to clockwise 
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }

    double DA=threshold+1;
    double prev=0;
    int countDA=0;
    while(DA>threshold){

        int initialArea=abs(polygon->area());
        vector<EdgeChange*> changes;
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
                if(checkPath(polygon,vi,vi,ei))
                    continue;
                vector<Point> path;
                path.push_back(*vi);
                double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                changes.push_back(newChange);

                // until we have gone out of bounds do this
                for (VertexIterator vi2 = vi; vi2 != polygon->vertices_end(); ++vi2){
                    path.push_back(*vi2);

                    // check if the path is ok with the size
                    if(path.size()>L){
                        break;
                    }
                    if(checkPath(polygon,vi,vi2,ei))
                        continue;

                    double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                    EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                    changes.push_back(newChange);
                }

                    // do this when we have to start from the starting vertex again
                for (VertexIterator vi2 = polygon->vertices_begin(); vi2 != vi; ++vi2){
                    path.push_back(*vi2);

                    // check if the path is ok with the size
                    if(path.size()>L){
                        break;
                    }
                    if(checkPath(polygon,vi,vi2,ei))
                        continue;

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
        changeEdge(polygon,theChange);
        *finalArea = abs(polygon->area());
        DA = (double)abs(*finalArea-initialArea) ;
        cout<<"DA: "<<DA<<endl;
        cout<<"!---!"<<endl;
        prev=DA;
    }

}