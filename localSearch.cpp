#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/intersections.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/number_utils.h>
#include <CGAL/squared_distance_2.h>
#include "geoUtil.hpp"
#include "genericUtil.hpp"
#include "edgeChange.hpp"
#include "incremental.hpp"
#include "convexHull.hpp"
#include <CGAL/Kd_tree.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Fuzzy_iso_box.h>
#include "timeManager.hpp"

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


//typeOfOptimization=1: max area, typeOfOptimization=2: min area
int localSearch(Polygon* polygon, int typeOfOptimization, int threshold, int L, int* finalArea,int countPoints){
    // change to clockwise 
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }
    

    double DA=threshold+1;
    int countDA=0;
    while(DA>=threshold){
        if(checkCutOf())
            return -10;
        int initialArea=abs(polygon->area());
        vector<EdgeChange*> changes;
        if (countPoints<=10){
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
                    if(checkCutOf())
                        return -10;
                    int stop=0;
                    if(checkPath(polygon,vi,vi,ei))
                        continue;
                    vector<Point> path;
                    path.push_back(*vi);
                    double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                    EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                    changes.push_back(newChange);

                    // until we have gone out of bounds do this
                    for (VertexIterator vi2 = vi+1; vi2 != polygon->vertices_end(); ++vi2){
                        if(checkCutOf())
                            return -10;
                        path.push_back(*vi2);

                        // check if the path is ok with the size
                        if(path.size()>L){
                            break;
                        }
                        if(ei->point(0)==*vi2||ei->point(1)==*vi2){
                            stop=1;
                            break;
                        }
                        if(checkPath(polygon,vi,vi2,ei))
                            continue;

                        double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                        EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                        changes.push_back(newChange);
                    }
                    if(stop)
                        continue;
                        // do this when we have to start from the starting vertex again
                    for (VertexIterator vi2 = polygon->vertices_begin(); vi2 != vi; ++vi2){
                        if(checkCutOf())
                            return -10;
                        path.push_back(*vi2);

                        // check if the path is ok with the size
                        if(path.size()>L){
                            break;
                        }
                        if(ei->point(0)==*vi2||ei->point(1)==*vi2){
                            stop=1;
                            break;
                        }
                        if(checkPath(polygon,vi,vi2,ei))
                            continue;

                        double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                        EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                        changes.push_back(newChange);
                    }
                    if(stop)
                        continue;
                }
            }
        }
        else{
                vector<EdgeIterator>see;
                srand(time(NULL));
                while(see.size()<10){
                    int flag;
                    do{
                        if(checkCutOf())
                            return -10;
                        flag=0;
                        int random=rand()%(countPoints);
                        for(int i=0;i<see.size();i++){
                            if(see.at(i)==polygon->edges_begin()+random){
                                flag=1;
                                break;
                            }
                        }
                        if (flag==0){
                            see.push_back(polygon->edges_begin()+random);
                        }
                    }while(flag==1);
                }
                for(int i=0; i<see.size(); i++){
                    //EdgeIterator ei=see.back();
                    EdgeIterator ei=see.at(i);
                    for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
                        if(checkCutOf())
                            return -10;
                        int stop=0;
                        if(checkPath(polygon,vi,vi,ei))
                            continue;
                        vector<Point> path;
                        path.push_back(*vi);
                        double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                        EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                        changes.push_back(newChange);

                        // until we have gone out of bounds do this
                        for (VertexIterator vi2 = vi+1; vi2 != polygon->vertices_end(); ++vi2){
                            if(checkCutOf())
                                return -10;
                            path.push_back(*vi2);

                            // check if the path is ok with the size
                            if(path.size()>L){
                                stop=1;
                                break;
                            }
                            if(ei->point(0)==*vi2||ei->point(1)==*vi2){
                                stop=1;
                                break;
                            }
                            if(checkPath(polygon,vi,vi2,ei))
                                continue;

                            double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                            EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                            changes.push_back(newChange);
                        }
                        if(stop)
                            continue;
                            // do this when we have to start from the starting vertex again
                        for (VertexIterator vi2 = polygon->vertices_begin(); vi2 != vi; ++vi2){
                            if(checkCutOf())
                                return -10;
                            path.push_back(*vi2);

                            // check if the path is ok with the size
                            if(path.size()>L){
                                break;
                            }
                            if(ei->point(0)==*vi2||ei->point(1)==*vi2){
                                stop=1;
                                break;
                            }
                            if(checkPath(polygon,vi,vi2,ei))
                                continue;

                            double area=calculateNewArea(polygon,*ei,path.front(),path.back(),&path);
                            EdgeChange* newChange = new EdgeChange(path.front(),path.back(),*ei,area);
                            
                            changes.push_back(newChange);
                        }
                        if(stop)
                            continue;
                    }
                }
                //}
        }
        if(changes.size() == 0){//no new changes could be found
            return -1;
        }
        int temp=changes.at(0)->getArea();
        EdgeChange* theChange=changes.at(0);
        
        for(int i=1;i<changes.size();i++){
            if(checkCutOf())
                return -10;
            // max
            if (typeOfOptimization==1 && temp<changes.at(i)->getArea() ){
                temp=changes.at(i)->getArea();                                                                                                                                                                                    
                theChange=changes.at(i);
            }
            // min
            else if (typeOfOptimization==2 && temp>changes.at(i)->getArea()){
                temp=changes.at(i)->getArea();
                theChange=changes.at(i);
            }
        }
        if(typeOfOptimization == 1){
            if(theChange->getArea() < 0)
                return 0;
        }
        else if(typeOfOptimization == 2){
            if(theChange->getArea() > 0)
                return 0;
        }
        
        changeEdge(polygon,theChange, countPoints);
        *finalArea = abs(polygon->area());
        DA = abs(theChange->getArea());
    }
    return 0;

}