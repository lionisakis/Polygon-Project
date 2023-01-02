#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "outputInfo.hpp"

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
const double outputInfo::getMinScore(){
    return this->minScore;
}
const double outputInfo::getMaxScore(){
    return this->maxScore;
}
const double outputInfo::getMaxBound(){
    return this->maxBound;
}
const double outputInfo::getMinBound(){
    return this->minBound;
}

void outputInfo::setMinScore(double sc){
    this->minScore+=sc;
}
void outputInfo::setMaxScore(double sc){
    this->maxScore+=sc;
}
void outputInfo::setMaxBound(double sc){
    if(sc<maxBound)
        this->maxBound = sc;
}
void outputInfo::setMinBound(double sc){
    if(sc>minBound)
        this->minBound = sc;
}

void outputInfo::printInfo(ofstream* outfile){
    *outfile << "||\t" << this->minScore <<"\t"<< this->maxScore <<"\t"<< this->minBound <<"\t"<< this->maxBound <<"\t";

}