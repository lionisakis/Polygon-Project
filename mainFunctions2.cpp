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
        *finalArea = abs(polygon->area());
        DA = abs(theChange->getArea());

    }

}
void globalStep(Polygon* polygon, int typeOfOptimization, double L, int* finalArea, int countPoints, int initialEnergy, int chArea, Point* mostLeft, Point* mostRight, int flagSub=0){
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
        
        //check if st is one of the marked edges
        if(flagSub){
            if(*s == *mostRight)
                valid=0;
            if(*t == *mostLeft)
                valid=0;
        }

        if(valid){
            cout <<"valid" << endl;
            currArea = abs(polygon->area());
            if(typeOfOptimization == 1)
                currEnergy = maxEnergy(countPoints, currArea, chArea);
            else if(typeOfOptimization == 2)
                currEnergy = minEnergy(countPoints, currArea, chArea);
            double DE = currEnergy - prevEnergy;
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
                    cout <<"not simple" << endl;
                    return;
                }
            }

            T=T-1/L;
            cout <<"T= " << T<<endl;
            prevEnergy = currEnergy;
            *finalArea=currArea;
        }
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
        Fuzzy_box default_range1(*p,*r);
        std::vector<Point> result;
        kd->search(std::back_inserter( result ), default_range1);
        Fuzzy_box default_range2(*q,*s);
        kd->search(std::back_inserter( result ), default_range2);
        
        int flag=0;
        // check if there is a problem or not
        for(int i=0;i<result.size();i++){
            Point x=result.at(i);
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                EdgeIterator seen=polygon->edges_end();
                if(x==*s&&ei->point(1)==*s){
                    seen=ei;
                }
                else if(x!=*s&&x!=*r&&x!=*q&&x!=*p){
                    seen=ei;
                }
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
            for(EdgeIterator ei=polygon->edges_begin();ei!=polygon->edges_end();ei++){
                EdgeIterator seen=polygon->edges_end();
                if(x==*p&&ei->point(0)==*p){
                    seen=ei;
                }
                else if(x!=*s&&x!=*r&&x!=*q&&x!=*p){
                    seen=ei;
                }
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
    cout<<"Return:"<<*q<<endl;
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
        if(q==polygon->vertices_end())
            break;
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

        cout<<"simple = "<<polygon->is_simple()<<endl;
        if(polygon->is_simple()==0)
            break;
        cout<<"-----\n";
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
        globalStep(polygon, typeOfOptimization, L, finalArea, countPoints, initialEnergy, chArea, NULL, NULL);
    }
}

//greedyAlgo=1: incremental, greedyAlgo=2: convex hull
//greedyEdge=1: random, =2:min, =3:max
void subdivision(Polygon* polygon, vector<Point>* points, int typeOfOptimization, double L, int *finalArea, int countPoints, int chArea, int greedyAlgo, int greedyEdge, int m){
    sortPoints(points, 2);
    double area;
    int count=0;
    int k = (int)ceil((double)(countPoints)/(double)(m-1));
    int flags[k];
    vector<Polygon> polygons(k);
    for(int i=0; i<k; i++){
        vector<Point> points2;
        //when you make the 2nd, 3rd, ..., kth polygon include also the previous last point
        if(i!=0){
            cout<<"Count:"<<count-1<<endl;
            points2.push_back(points->at(count-1));
        }
        
        //when we are not in the last subset add m points
        if(i<k-1){
            int temp=0;
            vector<Point> temporary;
            for(int j=0; j<m-1; j++){
                temporary.push_back(points->at(temp));
                temp++;
            }
            vector<Point> UH;
            vector<Point> LH;
            CGAL::upper_hull_points_2(temporary.begin(), temporary.end(), back_inserter(UH));
            CGAL::lower_hull_points_2(temporary.begin(), temporary.end(), back_inserter(LH));
            Segment mostRight=Segment(LH.at(LH.size()-1),UH.at(0));

            for(int j=0; j<m-1; j++){

                points2.push_back(points->at(count));
                count++;

            }
        }
        //when we are in the second to last subset and the remaining points are less than 3 add them to this one
        if((i==k-2) && (((countPoints-1)-(k-1)*(m-1)))<3){
            while(count<countPoints){
                points2.push_back(points->at(count));
                count++;
            }
        }
        //we are in the last subset and we have sufficient remaining points 
        else if(i==k-1 && (((countPoints-1)-(k-1)*(m-1)))>=3){
            while(count<countPoints){
                points2.push_back(points->at(count));
                count++;
            }
        }
        //we skip this subset cause all of the points were added to the previous one
        else if(i==k-1){
            cout<<i<<endl;
            polygons.pop_back();
            cout<<polygons.size()<<endl;
            break;
        }

        /* find if it is \/ or it is /\ */ 
        int flag=1;
        if(i!=0){
            Point p=points->at(count-m-1);
            Point q=points2.at(0);
            Point r=points2.at(1);
            cout<<"P:"<<p<<" Q:"<<q<<" R:"<<r<<endl;
            /* it is this /\ */
            if(p.y()<q.y() && r.y()<q.y()){
                flag=1;
            }
            /* it is this \/ */ 
            else if (p.y()>q.y() && r.y()>q.y()){
                flag=2;
            }
        }
        
        flags[i]=flag;
        cout<<"i= " << i<<" flag="<<flag<<endl;

        cout << "size of subset =  " << points2.size() << endl;
        if(greedyAlgo==1){
            incremental(&polygons.at(i), &points2, 2, greedyEdge, &area, flag);
        }
        else{
            convexHull(&polygons.at(i), &points2, greedyEdge, &area, flag);
        }

        //find leftmost, rightmost point of subset so that we dont break the marked edges
        vector<Point> LH;
        CGAL::lower_hull_points_2(points2.begin(), points2.end(), back_inserter(LH));
        vector<Point> UH;
        CGAL::upper_hull_points_2(points2.begin(), points2.end(), back_inserter(UH));

        Point mostLeft, mostRight;
        mostLeft = LH.at(0);
        mostRight = UH.at(0);

        //calculate initial energy of the greedy solution
        int initialEnergy;
        int pArea = abs(polygons.at(i).area());
        if(typeOfOptimization == 1)
            initialEnergy = maxEnergy(countPoints, pArea, chArea);
        else if(typeOfOptimization == 2)
            initialEnergy = minEnergy(countPoints, pArea, chArea);

        //apply global step to each subset
        // find KP 
        vector<Point> KP2;
        Polygon tmp;
        const Polygon::Vertices& range = polygons.at(i).vertices();
        CGAL::convex_hull_2(range.begin(), range.end(), back_inserter(KP2));
        for(int w=0; w<KP2.size(); w++)
            tmp.push_back(KP2.at(w));

        int chArea2 = abs(tmp.area());
        // globalStep(&polygons.at(i), typeOfOptimization, L, finalArea, points2.size(), initialEnergy, chArea2, &mostLeft, &mostRight, 1);
        // cout <<"done with global" << endl;
        // cout <<"---------" << endl;
        // for (EdgeIterator ei = polygons.at(i).edges_begin(); ei != polygons.at(i).edges_end(); ++ei)
        //     cout << *ei << endl;
        // cout <<"---------" << endl;
        cout << "simple =  " << polygons.at(i).is_simple() << endl;
    }
    
    cout<<"------------"<<endl;
    // just insert the first polygon
    for(VertexIterator vi=polygons.at(0).vertices_begin();vi!=polygons.at(0).vertices_end();vi++){
        polygon->push_back(*vi);
    }

    if(polygon->is_clockwise_oriented()==0){
        polygon->reverse_orientation();
    }
    
    for(EdgeIterator ei=polygon->edges_begin();polygon->edges_end()!=ei;ei++)
        cout<<*ei<<endl;
    cout<<"-----"<<endl;
    // for the other polygons
    for(int i=1;i<polygons.size();i++){
        // take the current polygon
        if(polygons.at(i).is_clockwise_oriented()==0){
            polygons.at(i).reverse_orientation();
        }
        Polygon* current=&polygons.at(i);

        // the polygon
        for(EdgeIterator ei=polygon->edges_begin();polygon->edges_end()!=ei;ei++)
            cout<<*ei<<endl;
        // the current
        for(EdgeIterator ei=current->edges_begin();current->edges_end()!=ei;ei++)
            cout<<*ei<<endl;

        cout<<"----"<<endl;
        // find p and q
        Point q, r;
        VertexIterator p,next;
        for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
            for(VertexIterator vCurrent=current->vertices_begin();vCurrent!=current->vertices_end();vCurrent++){
                if(*vAll==*vCurrent && flags[i]==1){
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

                    //find r
                    if(vCurrent==current->vertices_begin()){
                        r = *(current->vertices_end()-1);
                    }
                    else{
                        r = *(vCurrent-1);
                    }
                }
                else if(*vAll==*vCurrent && flags[i]==2){
                    // we found q=vALL=vCurrent
                    q=*vCurrent;
                    // if it is the last vertex
                    if(vAll==polygon->vertices_begin()){
                        p=polygon->vertices_end()-1;
                    }
                    else{
                        p=vAll-1;
                    }

                    // find next position of vCurrent
                    if(vCurrent==current->vertices_begin()){
                        next=current->vertices_end()-1;
                    }
                    else{
                        next=vCurrent-1;
                    }

                    //find r
                    if(vCurrent==current->vertices_end()-1){
                        r = *(current->vertices_begin());
                    }
                    else{
                        r = *(vCurrent+1);
                    }
                }
            }
        }
        
        // from [next,finish]
        int count=0;
        if(flags[i]==1){
            Point temp=*p;
            for(VertexIterator vCurrent2=next;vCurrent2!=current->vertices_end();vCurrent2++){
                cout<<"count= "<<count<<" temp:"<<temp<<endl;
                cout<<"count= "<<count<<" vCurrent2:"<<*vCurrent2<<endl;
                for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                    if(*vAll==temp){
                        polygon->insert(vAll,*vCurrent2);
                        if(polygon->is_simple()==0){
                            break;
                        }
                        break;
                    }
                }
                cout<<"Inserted\n";
                count++;
                //temp=*vCurrent2; //may need changing
            }
            cout<<"000"<<endl;
            // from [start,q)
            for(VertexIterator vCurrent2=current->vertices_begin();*vCurrent2!=q;vCurrent2++){
                cout<<"count= "<<count<<" p:"<<*p<<endl;
                cout<<"count= "<<count<<" vCurrent2:"<<*vCurrent2<<endl;
                for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                    if(*vAll==temp){
                        // cout<<*vAll<<endl;
                        polygon->insert(vAll,*vCurrent2);
                        break;
                    }
                }
                //temp=*vCurrent2;
                count++;
            }
        }
        else{
            Point temp=q;
            for(VertexIterator vCurrent2=next;vCurrent2!=current->vertices_begin();vCurrent2--){
                cout<<"count= "<<count<<" temp:"<<temp<<endl;
                cout<<"count= "<<count<<" vCurrent2:"<<*vCurrent2<<endl;
                for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                    if(*vAll==temp){
                        polygon->insert(vAll,*vCurrent2);
                        break;
                    }
                }
                cout<<"Inserted\n";
                count++;
                temp=*vCurrent2;
            }

            if(q!=*current->vertices_begin()){
                // vertexes begin
                cout<<"count= "<<count<<" temp:"<<temp<<endl;
                cout<<"count= "<<count<<" vCurrent2:"<<*current->vertices_begin()<<endl;
                for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                    if(*vAll==temp){
                        polygon->insert(vAll,*current->vertices_begin());
                        break;
                    }
                }
                cout<<"Inserted\n";
                count++;
                temp=*current->vertices_begin();
            }

            cout<<"000"<<endl;
            // from [end,q)
            int flag=0;
            for(VertexIterator vCurrent3=current->vertices_end()-1;*vCurrent3!=q ;vCurrent3--){
                for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                    if(*vAll==*vCurrent3){
                        flag=1;
                        break;
                    }
                }
                if(flag)
                    continue;
                cout<<"count= "<<count<<" temp:"<<temp<<endl;
                cout<<"count= "<<count<<" vCurrent3:"<<*vCurrent3<<endl;
                for(VertexIterator vAll=polygon->vertices_begin();vAll!=polygon->vertices_end();vAll++){
                    if(*vAll==temp){
                        polygon->insert(vAll,*vCurrent3);
                        break;
                    }
                }
                temp=*vCurrent3;
                count++;
            }
        }

        cout<<"i="<<i<<endl;
        cout<<"flag="<<flags[i]<<endl;
        cout<<"simple="<<polygon->is_simple()<<endl;
        cout<<"------"<<endl;
        if(polygon->is_simple()==0){
            for (EdgeIterator ei = polygon->edges_begin(); ei != polygon->edges_end(); ++ei)
                cout << *ei << endl;
            break;
        }
    }

}