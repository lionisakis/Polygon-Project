#include <iostream>
#include <cstdlib>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "edgeChange.hpp"
#include <math.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Segment_2<K> Segment;
typedef CGAL::Polygon_2<K> Polygon;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;


using namespace std;


//1: x decreasing 
//2: x increasing 
//3: y decreasing
//4: y increasing
void swap(Point* a, Point* b,int type){
    // x decreasing
    if (type==1){
        if(a->x()<b->x()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
    // x increasing
    else if (type==2){
        if(a->x()>b->x()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
    // y decreasing
    else if(type==3){
        if(a->y()<b->y()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
    // y increasing
    else{
        if(a->y()>b->y()){
            Point temp=*a;
            *a=*b;
            *b=temp;
        }
    }
}

//sort points in a vector based on type=
//1: x decreasing 
//2: x increasing 
//3: y decreasing
//4: y increasing
void sortPoints(vector<Point>* points, int type){
    for (int i=0;i<points->size();i++){
        for(int j=i+1;j<points->size();j++){
            swap(&points->at(i),&points->at(j),type);
        }
    }
}




//calculate energy for maximization problem
double minEnergy(int n , int polygonArea, int chArea){
    double tmp = (double)polygonArea/(double)chArea;
    return n*tmp;
}

//calculate energy for minimization problem
double maxEnergy(int n, int polygonArea ,int chArea){
    double tmp = (double)polygonArea/(double)chArea;
    return n*(1-tmp);
}

//see if metropolis criterion holds
int Metropolis(double DE,double T){    
    double R =(double)rand() / (double)((unsigned)RAND_MAX + 1);
    return exp(-DE/T)>=R;
}