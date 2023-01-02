#include <iostream>
#include <vector>
#include "outputInfo.hpp"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

using namespace std;

outputInfo::outputInfo(int n){
    size = n;
    minScore = 0;
    maxScore = 0;
    minBound = 0;
    maxBound = INFINITY;
}

const double outputInfo::getSize(){
    return this->size;
}
const double outputInfo::getminScore(){
    return this->minScore;
}
const double outputInfo::getmaxScore(){
    return this->maxScore;
}
const double outputInfo::getmaxBound(){
    return this->maxBound;
}
const double outputInfo::getminBound(){
    return this->minBound;
}

void outputInfo::setminScore(double sc){
    this->minScore+=sc;
}
void outputInfo::setmaxScore(double sc){
    this->maxScore+=sc;
}
void outputInfo::setmaxBound(double sc){
    if(sc<maxBound)
        this->maxBound = sc;
}
void outputInfo::setminBound(double sc){
    if(sc>minBound)
        this->minBound = sc;
}

void outputInfo::printInfo(ofstream* outfile){
    *outfile << "||\t" << this->minScore <<"\t"<< this->maxScore <<"\t"<< this->minBound <<"\t"<< this->maxBound <<"\t";

}