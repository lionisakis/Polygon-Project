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

int checkPath(Polygon* polygon,VertexIterator viPathFirst,VertexIterator viPathLast,EdgeIterator ei2){
    cout <<"q = " << *viPathLast << endl;
    cout <<"st = " << *ei2 << endl;
    sleep(3);
    // and not in our edge
    if(ei2->point(0)==*viPathLast||ei2->point(1)==*viPathLast){
        return 1;
    }
    // check visibility that the last point is visible
    if ((checkVisibility(polygon,*viPathLast,ei2->point(0))==0)||(checkVisibility(polygon,*viPathLast,ei2->point(1)))==0)
        return 1;
    
    // check the change does not change our simplicity
    Segment segLeft=Segment(ei2->point(0),*viPathFirst);
    Segment segRight=Segment(*viPathLast,ei2->point(1));
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
//typeOfOptimization=1: max area, typeOfOptimization=2: min area
void localSearch(Polygon* polygon, int typeOfOptimization, int threshold, int L, int* finalArea,int countPoints){
    // change to clockwise 
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }

    double DA=threshold+1;
    int countDA=0;
    while(DA>threshold){

        int initialArea=abs(polygon->area());
        vector<EdgeChange*> changes;
        if (countPoints<=10){
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
                    int stop=0;
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
        else {
            
            while(changes.size() == 0){
                vector<EdgeIterator>see;
                while(see.size()<=10){
                    int flag;
                    do{
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
                    EdgeIterator ei=see.back();
                    for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
                        int stop=0;
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
        }
        //cout << "vector size " << changes.size() << endl;
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
        changeEdge(polygon,theChange, countPoints);
        *finalArea = abs(polygon->area());
        DA = abs(theChange->getArea());
        // cout<<"DA: "<<DA<<endl;
        if(polygon->is_simple()==0){
            cout <<"l " << L << endl;
            cout << "left " << theChange->getLeft() << endl;
            cout << "right " << theChange->getRight() << endl;
            cout << "edge " << theChange->getSegment() << endl;
            cout <<"changes = " << changes.size() << endl;
            cout <<"not simple" << endl;
            break;
        }
        cout << "simple = " << polygon->is_simple() << endl;
        // cout<<"!---!"<<endl;
        
    }

}
void globalStep(Polygon* polygon, int typeOfOptimization, int L, int* finalArea, int countPoints, int initialEnergy, int chArea,double R){
    
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }
    double T=1;
    int currEnergy;
    int prevEnergy=initialEnergy;
    while (T>=0){

        //randomly find points q,s
        int currArea;
        int q1 = rand()%(countPoints);
        int s1;
        int flag;
        do{
            flag=0;
            int random=rand()%(countPoints);
            if(q1==random){
                flag=1;
            }
            if (flag==0){
                s1=random;
            }
        }while(flag==1);
        sleep(3);
        int tmp=0;
        VertexIterator q, s, p, t,r;
        for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
            if(tmp == q1){
                q = vi;
                if (q==polygon->vertices_end()-1)
                    r=polygon->vertices_begin();
                else
                    r=q+1;
                
                if (q==polygon->vertices_begin())
                    p=polygon->vertices_end()-1;
                else
                    p=q-1;
            }
            else if(tmp == s1){
                s = vi;
                if (s==polygon->vertices_end()-1)
                    t=polygon->vertices_begin();
                else
                    t=s+1;
            }
            tmp++;
        }
        EdgeIterator st;//edge to be broken
        for (EdgeIterator ei = polygon->edges_begin(); ei != polygon->edges_end(); ++ei){
            if(ei->point(0) == *s && ei->point(1) == *t){
                st = ei;
                break;
            }
        }

        //check validity
        int valid=0;
        if(checkPath(polygon, q, q, st) == 0)
            valid=1;

        if(valid){
            cout <<"transition" << endl;
            currArea = abs(polygon->area());
            if(typeOfOptimization == 1)
                currEnergy = maxEnergy(countPoints, currArea, chArea);
            else if(typeOfOptimization == 2)
                currEnergy = minEnergy(countPoints, currArea, chArea);
            int DE = currEnergy - prevEnergy;
            if(DE < 0 || Metropolis(DE,T,R)){//make function for metropolis
                Point tmp = *q;
                polygon->erase(q);
                polygon->insert(t, tmp);
            }

            T=T-1/L;
            prevEnergy = currEnergy;
        }
        cout <<"searching " << endl;
    }
}

VertexIterator localAlgorithm(Polygon* polygon, Tree* kd,int countPoints){
    vector<int> seen;
    int flag=1;
    VertexIterator p,q,r,s;
    // find a random point until there is no problem
    while(flag==1){
        cout<<"---"<<endl;
        // find a random point that you have not seen yet
        int notSeen=0;
        int pos;
        while(notSeen==0){
            pos=rand()%countPoints;
            int i;
            for(i=0;i<seen.size();i++){
                if (seen.at(i)==pos)
                    break;
            }
            if(i==seen.size()){
                notSeen=1;
                seen.push_back(pos);
            }
        }
        q=polygon->vertices_begin()+pos;
        
        // find the next point
        if (q==polygon->vertices_end()-1)
            r=polygon->vertices_begin();
        else
            r=q+1;
        
        if (q==polygon->vertices_begin())
            p=polygon->vertices_end()-1;
        else
            p=q-1;

        // find the next point
        if (r==polygon->vertices_end()-1)
            s=polygon->vertices_begin();
        else
            s=q+1;
        

        // do the search
        Fuzzy_box default_range1(*p,*r);
        std::vector<Point> result;
        kd->search(std::back_inserter( result ), default_range1);
        cout<<result.size()<<endl;
        cout<<"default_range1 "<<*p<<":"<<*r<<endl;
        // check if there is a problem or not
        for(int i=0;i<result.size();i++){
            cout<<result.at(i)<<endl;
        }
        Fuzzy_box default_range2(*q,*s);
        kd->search(std::back_inserter( result ), default_range2);
        cout<<result.size()<<endl;
        cout<<"default_range2 "<<*q<<":"<<*s<<endl;
        // check if there is a problem or not
        for(int i=0;i<result.size();i++){
            Point x=result.at(i);
            if (x==*q||x==*r||x==*p||x==*s){
                continue;
            }
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                
            }
        }
        // check if there is a problem or not
        if (result.size()==2)
            flag=0;
    }
    return q;
}

void localMinimum(Polygon* polygon,int typeOfOptimization, int L, int* finalArea, int countPoints, int initialEnergy, int chArea,double R){
     // change to clockwise 
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }

    Tree kd;
    for(VertexIterator vi=polygon->vertices_begin(); vi!=polygon->vertices_end();vi++)
        kd.insert(*vi);
    
    double T=1;
    int currEnergy;
    int prevEnergy=initialEnergy;
    while (T>=0){
        VertexIterator q=localAlgorithm(polygon,&kd,countPoints);
        VertexIterator r;
        if (q==polygon->vertices_end()-1)
            r=polygon->vertices_begin();
        else
            r=q+1;


        // find the energy
        int currArea=abs(polygon->area());
        if(typeOfOptimization == 1)
            currEnergy = maxEnergy(countPoints, currArea, chArea);
        else if(typeOfOptimization == 2)
            currEnergy = minEnergy(countPoints, currArea, chArea);


        // do the transition
        int DE = currEnergy - prevEnergy;
        if(DE < 0 || Metropolis(DE,T,R)){//make function for metropolis
            Point temp=*r;
            polygon->erase(r);
            polygon->insert(q,temp);
        }

        cout<<"simple = "<<polygon->is_simple()<<endl;
        if(polygon->is_simple()==0)
            break;
        cout<<"-----\n";
        T=T-1/L;
    }    
}

//typeOfOptimization=1: max, =2:min
//typeOfStep=1: local step, =2: global step, =3:subdivision
void simulated_annealing(Polygon* polygon, int typeOfOptimization, int L, int* finalArea,int countPoints, int typeOfStep, int initialEnergy, int chArea,double R){
    //local step
    
    if(typeOfStep==1){
        localMinimum(polygon,typeOfOptimization, L, finalArea, countPoints, initialEnergy, chArea,R);
    }
    //global step
    else if(typeOfStep==2){
        globalStep(polygon, typeOfOptimization, L, finalArea, countPoints, initialEnergy, chArea,R);

    }
    //subdivision
    else if (typeOfStep == 3){

    }

}