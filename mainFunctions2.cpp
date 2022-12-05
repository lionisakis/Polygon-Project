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
                    for (VertexIterator vi2 = vi+1; vi2 != polygon->vertices_end(); ++vi2){
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
        else{
            //while(changes.size() == 0){
                vector<EdgeIterator>see;
                while(see.size()<10){
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
                }
                for(int i=0; i<see.size(); i++){
                    //EdgeIterator ei=see.back();
                    EdgeIterator ei=see.at(i);
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
                        for (VertexIterator vi2 = vi+1; vi2 != polygon->vertices_end(); ++vi2){
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
                //}
            //}
        }
        if(changes.size() == 0){//no new changes could be found
            return;
        }
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
        
        changeEdge(polygon,theChange, countPoints);
        cout <<"init area = " << initialArea << endl;
        *finalArea = abs(polygon->area());
        cout <<"final area = " << *finalArea << endl;
        DA = abs(theChange->getArea());
        cout <<"DA = " << DA << endl;
        if(polygon->is_simple() ==0 ){
            cout << "simplicity broken" << endl;
            exit(1);
        }
        
    }

}

//typeOfOptimization=1:max, =2:min
//flagSub=1: called for subdivison-have to maintain certain edges =0: no subdivision
void globalStep(Polygon* polygon, int typeOfOptimization, double L, int* finalArea, int countPoints, int initialEnergy, int chArea, int flagSub=0){
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }
    Segment left,right;
    Point mostLeft;
    Point mostRight;
    int totalIterations=0;
    if(flagSub){
        mostLeft =*polygon->left_vertex();
        mostRight =*polygon->right_vertex();
        for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            if(ei->point(1)==mostLeft){
                left=*ei;
            }
            if(ei->point(0)==mostRight){
                right=*ei;
            }
        }   
    }
    double T=1;
    int currEnergy;
    int prevEnergy=initialEnergy;
    while (T>=0){
        //put an upper bound on how many random choices global step can make sp that we avoid an infinite loop
        if(totalIterations > 2*L){
            return;
        }
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
        int tmp=0;
        VertexIterator q, s, p, t,r;
        int tr=0;
        for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
            tr++;
        }
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
        EdgeIterator ei;
        for (ei = polygon->edges_begin(); ei != polygon->edges_end(); ++ei){
            if(ei->point(0) == *s && ei->point(1) == *t){
                st = ei;
                break;
            }
        }
        //check validity
        int valid=0;
        if(checkPath(polygon, q, q, st) == 0)
            valid=1;
        
        //check if st is one of the marked edges
        if(flagSub){
            if(*s == mostRight)
                valid=0;
            if(*t == mostLeft)
                valid=0;
            if(mostRight == *q)
                valid=0;
            if(mostLeft == *q)
                valid=0;
            if(mostRight == *p)
                valid=0;
            if(mostLeft == *p)
                valid=0;
            if(mostRight == *r )
                valid=0;
            if(mostLeft == *r)
                valid=0;
        }
        if(valid){
            currArea = abs(polygon->area());
            if(typeOfOptimization == 1)
                currEnergy = maxEnergy(countPoints, currArea, chArea);
            else if(typeOfOptimization == 2)
                currEnergy = minEnergy(countPoints, currArea, chArea);
            double DE = currEnergy - prevEnergy;
            if(polygon->is_simple()==0){
                cout <<"not simple2 prev global" << endl;
                return;
            }
            if(DE < 0.0 || Metropolis(DE,T)){//make function for metropolis
                Point tmp = *q;
                Point tmp2 = *t;
                polygon->erase(q);
                for (VertexIterator vi = polygon->vertices_begin(); vi != polygon->vertices_end(); ++vi){
                    if(*vi == tmp2){
                        polygon->insert(vi, tmp);
                        break;
                    }
                }
                if(polygon->is_simple()==0){
                    cout <<"not simple2 after global" << endl;
                    return;
                }
            }

            T=T-1/L;
            prevEnergy = currEnergy;
            *finalArea=currArea;
        }
        totalIterations++;
    }
}


VertexIterator localAlgorithm(Polygon* polygon, Tree* kd,int countPoints){
    vector<int> seen;
    VertexIterator p,q,r,s;
    // find a random point until there is no problem
    while(1){
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
            if(seen.size()==10){
                return polygon->vertices_end();
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
            s=r+1;
        
        Segment qs=Segment(*q,*s);
        Segment pr=Segment(*p,*r);
        if(intersection(qs,pr))
            continue;

        // do the search
        Fuzzy_box default_range1(*q,*s);
        std::vector<Point> result;
        kd->search(std::back_inserter( result ), default_range1);
        Fuzzy_box default_range2(*p,*r);
        kd->search(std::back_inserter( result ), default_range2);
        Fuzzy_box default_range3(*p,*q);
        kd->search(std::back_inserter( result ), default_range3);
        Fuzzy_box default_range4(*r,*s);
        kd->search(std::back_inserter( result ), default_range4);
        Fuzzy_box default_range5(*p,*s);
        kd->search(std::back_inserter( result ), default_range5);
        Fuzzy_box default_range6(*r,*q);
        kd->search(std::back_inserter( result ), default_range6);

        
        int flag=0;
        // check if there is a problem or not
        for(int i=0;i<result.size();i++){
            Point x=result.at(i);
            if(x==*r || x==*q || x==*p){
                continue;
            }
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                EdgeIterator seen=polygon->edges_end();
                
                if(x ==*s && *s==ei->point(1))
                    continue;
                if(x ==*p && *p==ei->point(0))
                    continue;
                if(x == ei->point(0) || x == ei->point(1))
                    seen = ei;
                else
                    continue;
                
                if(seen!=polygon->edges_end()){
                    if(intersection(pr,*seen)){
                        flag=1;
                        break;
                    }
                    
                }
            }
        }
        if (flag==1)
            continue;  
        
        // check if there is a problem or not
        flag=0;

        for(int i=0;i<result.size();i++){
            Point x=result.at(i);
            if(x==*q || x==*r || x==*s){
                continue;
            }
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                EdgeIterator seen=polygon->edges_end();
                if(x ==*s && *s==ei->point(1))
                    continue;
                if(x ==*p && *p==ei->point(0))
                    continue;
                if(x == ei->point(0) || x == ei->point(1))
                    seen = ei;
                else
                    continue;
                if(seen!=polygon->edges_end()){
                    if(intersection(qs,*seen)){
                        flag=1;
                        break;
                    }
                }
            }
        }

        if (flag==1)
            continue;
        
        break;
    }
    return q;
}

void localMinimum(Polygon* polygon,int typeOfOptimization, double L, int* finalArea, int countPoints, int initialEnergy, int chArea){
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
        if(q==polygon->vertices_end()){
            T = T-1/L;
            continue;
        }
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
        double DE = currEnergy - prevEnergy;
        if(DE < 0 || Metropolis(DE,T)){//make function for metropolis
            Point temp=*r;
            Point x=*q;
            polygon->erase(r);
            for(VertexIterator vi=polygon->vertices_begin();vi!=polygon->vertices_end();vi++){
                if(*vi==x){
                    polygon->insert(vi,temp);
                    break;
                }
            }
        }

        if(polygon->is_simple()==0)
            break;
        T=T-1/L;
        *finalArea=currArea;
        prevEnergy = currEnergy;
    }    

}

//typeOfOptimization=1: max, =2:min
//typeOfStep=1: local step, =2: global step
void simulated_annealing(Polygon* polygon, int typeOfOptimization, double L, int* finalArea,int countPoints, int typeOfStep, int initialEnergy, int chArea){
    //local step
    if(typeOfStep==1){
        localMinimum(polygon,typeOfOptimization, L, finalArea, countPoints, initialEnergy, chArea);
    }
    //global step
    else if(typeOfStep==2){
        globalStep(polygon, typeOfOptimization, L, finalArea, countPoints, initialEnergy, chArea);
    }
}

//greedyAlgo=1: incremental, greedyAlgo=2: convex hull
//greedyEdge=1: random, =2:min, =3:max
//m=[10,100]
void subdivision(Polygon* polygon, vector<Point>* points, int typeOfOptimization, double L, int *finalArea, int countPoints, int chArea, int greedyAlgo, int greedyEdge, int m, int* initialArea){
    sortPoints(points, 2);
    double area;
    int howManyToAdd=m-1;
    int k = (int)ceil((double)(countPoints)/(double)(howManyToAdd));
    vector<Polygon> polygons(k);
    vector<Point> nextPoints;
    int offset=0;
    int runs=0;
    for(int i=0; i<k; i++){
        // no more k
        if(offset>=countPoints-1)
            break;
        runs++;

        vector<Point> current;

        // check if the last point that you are going to add 
        // satisfies the conditions
        // countPoints-1 the last point and -3 as we do not want the last subset to be size =3
        int last;
        for(last=offset+howManyToAdd;last<countPoints-1-3;last++){
            // condition 1
            int flag=0;
            for(int j=offset;j<offset+howManyToAdd;j++){
                if(j==countPoints)
                    break;    
                if(points->at(j).y()<points->at(last).y()){
                    flag=1;
                    break;
                }
            }
            // it does not satisfy the condition
            if(!flag)
                continue;


            // condition 2
            flag=0;
            for(int j=offset+howManyToAdd+1;j<offset+2*howManyToAdd;j++){
                if(j==countPoints)
                    break;       
                if(points->at(last).y()>points->at(j).y()){
                    flag=1;
                    break;
                }
                         
            }
            // it does not satisfy the condition
            if(!flag)
                continue;

            // it satisfy the condition
            break;
        }
        int lastSet=0;
        if(last>=countPoints-1-3){
            last=countPoints-1;
            lastSet=1;
        }
        while(offset<=last){
            current.push_back(points->at(offset));
            offset++;
        }
        offset--;
        Segment segments[2];
        Point left=current.at(0);
        Point right=current.at(current.size()-1);
        int flagFirst=1;
        for(int i=1;i<current.size()-1;i++){
            if(current.at(i).y()<left.y()&&flagFirst){
                segments[0]=Segment(left,current.at(i));
                flagFirst=0;
            }
            else if(current.at(i).y()<right.y()){
                segments[1]=Segment(current.at(i),right);
            }
        }
        if(greedyAlgo==1){
            vector<Point>temp=current;
            
            int result;
            if(lastSet)
                result=incremental(&polygons.at(i), &temp, 2, greedyEdge, &area, 1, segments, 1);
            else
                result=incremental(&polygons.at(i), &temp, 2, greedyEdge, &area, 1, segments);
            if(result){
                //cout <<"incremental could not make requested polygon, convex hull is called" << endl;
                VertexIterator vi=polygons.at(i).vertices_begin();
                while(vi!=polygons.at(i).vertices_end())
                    polygons.at(i).erase(vi);
    
                int result;
                if(lastSet)
                    result=convexHull(&polygons.at(i), &current, greedyEdge, &area, 1, segments, 1);
                else
                    result=convexHull(&polygons.at(i), &current, greedyEdge, &area, 1, segments);
                if(result){
                    cout<<"The polygon was not made so I cannot continue."<<endl;
                    exit(1);
                }
            }
        }
        else{
            int result=convexHull(&polygons.at(i), &current, greedyEdge, &area, 1, segments);
            if(result){
                cout<<"The polygon was not made so I cannot continue."<<endl;
                exit(1);
            }
        }
        int o=0;
        for (VertexIterator vi = polygons.at(i).vertices_begin(); vi != polygons.at(i).vertices_end(); ++vi){
            o++;
        }
        if(polygons.at(i).is_simple()==0){
            cout <<"not simple!!! after greedy creation" << endl;
        }
        
        // if(i==8){
        //     cout <<"!s" << endl;
        //     for(EdgeIterator ei=polygons.at(i).edges_begin();ei!=polygons.at(i).edges_end();ei++){
        //         cout << *ei << endl;
        //     }
        //     cout <<"!e" << endl;
        // }
        
        Point mostLeft, mostRight;
        mostLeft = current.at(0);
        mostRight = current.at(current.size()-1);

        //apply global step to each subset
        // find KP to computer KP area for the energy function
        vector<Point> KP2;
        Polygon tmp;
        const Polygon::Vertices& range = polygons.at(i).vertices();
        CGAL::convex_hull_2(range.begin(), range.end(), back_inserter(KP2));
        for(int w=0; w<KP2.size(); w++)
            tmp.push_back(KP2.at(w));

        int chArea2 = abs(tmp.area());

        //calculate initial energy of the greedy solution
        int initialEnergy;
        int pArea = abs(polygons.at(i).area());
        if(typeOfOptimization == 1)
            initialEnergy = maxEnergy(countPoints, pArea, chArea2);
        else if(typeOfOptimization == 2)
            initialEnergy = minEnergy(countPoints, pArea, chArea2);
        int trsize=0;
        for(VertexIterator vi = polygons.at(i).vertices_begin(); vi!=polygons.at(i).vertices_end(); vi++){
            trsize++;
        }
        //apply global step
        globalStep(&polygons.at(i), typeOfOptimization, L, finalArea, current.size(), initialEnergy, chArea2, 1);
    }
    
    // just insert the first polygon
    for(VertexIterator vi=polygons.at(0).vertices_begin();vi!=polygons.at(0).vertices_end();vi++){
        polygon->push_back(*vi);
    }
    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }
    
    // for the other polygons
    for(int i=1;i<runs;i++){
        // take the current polygon
        Polygon* current=&polygons.at(i);
        if(current->is_clockwise_oriented()==0){
            current->reverse_orientation();
        }
    
        // find p and q
        Point q, r;
        VertexIterator p,next;
        for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
            for(VertexIterator vCurrent=current->vertices_begin();vCurrent!=current->vertices_end();vCurrent++){
                if(*vAll==*vCurrent ){
                    // we found q=vALL=vCurrent
                    q=*vCurrent;
                    // if it is the last vertex
                    if(vAll==polygon->vertices_end()-1){
                        p=polygon->vertices_begin();
                    }
                    else{
                        p=vAll+1;
                    }

                    // find next position of vCurrent
                    if(vCurrent==current->vertices_end()-1){
                        next=current->vertices_begin();
                    }
                    else{
                        next=vCurrent+1;
                    }
                }
            }
        }

        int count=0;
        Point temp=*p;
        int flagQ=0;
        // [next,finish) break to q
        for(VertexIterator vCurrent2=next;vCurrent2!=current->vertices_end();vCurrent2++){
            if(*vCurrent2==q){
                flagQ=1;
                break;
            }
            for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                if(*vAll==temp){
                    polygon->insert(vAll,*vCurrent2);
                    break;
                }
            }
            count++;
        }

        // from [start,q)
        if(flagQ==0){
            for(VertexIterator vCurrent2=current->vertices_begin();*vCurrent2!=q;vCurrent2++){
                for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                    if(*vAll==temp){
                        polygon->insert(vAll,*vCurrent2);
                        break;
                    }
                }
                count++;
            }
        }
        
        if(polygon->is_simple()==0){
            // for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
            //     cout << *ei << endl;
            // }
            cout <<"problem and i = "<< i << endl;
            exit(10);
        }

    }
    
    //apply local step 
    int initialEnergy;
    *initialArea = abs(polygon->area());
    if(typeOfOptimization == 1)
        initialEnergy = maxEnergy(countPoints, *initialArea, chArea);
    else if(typeOfOptimization == 2)
        initialEnergy = minEnergy(countPoints, *initialArea, chArea);
    localMinimum(polygon,typeOfOptimization,L,finalArea,countPoints,initialEnergy,chArea);

}