#include "runCases.hpp"
#include  "incremental.hpp"
#include "convexHull.hpp"
#include "localSearch.hpp"
#include "simulatedAnnealing.hpp"
#include "outputInfo.hpp"
#include "timeManager.hpp"
#include "genericUtil.hpp"
#include "mainUtil.hpp"
#include "allIncludes.hpp"

// subdivision -> localSearch
void runCase1(vector<Point>* allPoints, vector<outputInfo *>* infoCase1, double chArea){
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    double minBound;
    double maxBound;

    //first for minimization
    Polygon pMin;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    initializeTime(allPoints->size());
    int finalRes;
    int flagCont=1;
    
    double L=1000;
    int m=100;
    if(subdivision(&pMin, allPoints, 2, L, &pArea2, allPoints->size(), chArea, 2, 1, m, &pArea) == -10)//3rd argument from end either 2 or 1 (min or random edge selection)
        flagCont=0;

    if(flagCont){
        int threshold=1;
        L=10;
        finalRes = localSearch(&pMin, 2, threshold, L, &pArea2, allPoints->size());
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    

    //then for maximization
    flagCont=1;
    Polygon pMax;
    ourArea=0;
    pArea=0;
    pArea2=0;//in this variable we store the area calculated by our algorithm

    initializeTime(allPoints->size());
    double L=10;
    int m=100;
    if(subdivision(&pMin, allPoints, 1, L, &pArea2, allPoints->size(), chArea, 2, 3, m, &pArea) == -10)//3rd argument from end either 3 or 1 (max or random edge selection)
        flagCont=0;

    if(flagCont){
        int threshold=1;
        L=10;
        finalRes = localSearch(&pMax, 1, threshold, L, &pArea2, allPoints->size());
    }


    if(flagCont == 0 || finalRes == -10)
        maxScore=0;
    else
        maxScore = (double)pArea2/(double)chArea;

    //update info case vector with the new scores/bounds
    for(int i=0; i<infoCase1->size(); i++){
        if(infoCase1->at(i)->getSize() == allPoints->size()){
            infoCase1->at(i)->setMinScore(minScore);
            infoCase1->at(i)->setMinBound(minScore);
            infoCase1->at(i)->setMaxScore(maxScore);
            infoCase1->at(i)->setMaxBound(maxScore);
            break;
        }
    }    
}

//incremental->localStep->localSearch
void runCase2(vector<Point>* allPoints, vector<outputInfo *>* infoCase2, double chArea){
    double L;
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    double minBound;
    double maxBound;

    //first for minimization
    Polygon pMin;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    initializeTime(allPoints->size());
    int finalRes;
    int flagCont=1;
    if(incremental(&pMin, allPoints, 1, 2, &ourArea) == -10)//4th argument either 2 or 1 (min or random edge selection)
        flagCont=0;
    pArea = abs(pMin.area());
    if(flagCont){
        L=1000;
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
        int threshold=1;
        L=10;
        finalRes = localSearch(&pMin, 2, threshold, L, &pArea2, allPoints->size());
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    

    //then for maximization
    flagCont=1;
    Polygon pMax;
    ourArea=0;
    pArea=0;
    pArea2=0;//in this variable we store the area calculated by our algorithm

    initializeTime(allPoints->size());
    if(incremental(&pMax, allPoints, 1, 3, &ourArea) == -10)//4th argument either 3 or 1 (max or random edge selection)
        flagCont=0;
    pArea = abs(pMax.area());
    if(flagCont){
        L=1000;
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
        int threshold=1;
        L=10;
        finalRes = localSearch(&pMax, 1, threshold, L, &pArea2, allPoints->size());
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        maxScore=0;
    else
        maxScore = (double)pArea2/(double)chArea;

    //update info case vector with the new scores/bounds
    for(int i=0; i<infoCase2->size(); i++){
        if(infoCase2->at(i)->getSize() == allPoints->size()){
            infoCase2->at(i)->setMinScore(minScore);
            infoCase2->at(i)->setMinBound(minScore);
            infoCase2->at(i)->setMaxScore(maxScore);
            infoCase2->at(i)->setMaxBound(maxScore);
            break;
        }
    }
}

//convexHull->globalStep->localStep
void runCase3(vector<Point>* allPoints, vector<outputInfo *>* infoCase3, double chArea){
    double L;
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    double minBound;
    double maxBound;

    //first for minimization
    Polygon pMin;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    initializeTime(allPoints->size());
    int finalRes;
    int flagCont=1;
    if(convexHull(&pMin, allPoints, 2, &ourArea) == -10)//3rd argument either 2 or 1 (min or random edge selection)
        flagCont=0;
    pArea = abs(pMin.area());

    if(flagCont){
        L=1000;
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(globalStep(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }

    if(flagCont){
        L=1000;
        int initialEnergy2 = minEnergy(allPoints->size(), pArea2, chArea);
        finalRes = localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy2, chArea);
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    

    //then for maximization
    flagCont=1;
    Polygon pMax;
    ourArea=0;
    pArea=0;
    pArea2=0;//in this variable we store the area calculated by our algorithm

    initializeTime(allPoints->size());
    if(convexHull(&pMax, allPoints, 3, &ourArea) == -10)//3rd argument either 3 or 1 (max or random edge selection)
        flagCont=0;
    pArea = abs(pMax.area());
    
    if(flagCont){
        L=1000;
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(globalStep(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }

    if(flagCont){
        L=1000;
        int initialEnergy2 = maxEnergy(allPoints->size(), pArea2, chArea);
        finalRes = localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy2, chArea);
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        maxScore = 0;
    else
        maxScore = (double)pArea2/(double)chArea;

    //update info case vector with the new scores/bounds
    for(int i=0; i<infoCase3->size(); i++){
        if(infoCase3->at(i)->getSize() == allPoints->size()){
            infoCase3->at(i)->setMinScore(minScore);
            infoCase3->at(i)->setMinBound(minScore);
            infoCase3->at(i)->setMaxScore(maxScore);
            infoCase3->at(i)->setMaxBound(maxScore);
            break;
        }
    }
}

// incremental->localStep->globalStep
void runCase4(vector<Point>* allPoints, vector<outputInfo *>* infoCase4, double chArea){
    double L;
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    double minBound;
    double maxBound;

    //first for minimization
    Polygon pMin;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    initializeTime(allPoints->size());
    int finalRes;
    int flagCont=1;
    if(incremental(&pMin, allPoints, 1, 2, &ourArea) == -10)//4th argument either 2 or 1 (min or random edge selection)
        flagCont=0;
    pArea = abs(pMin.area());
    if(flagCont){
        L=1000;
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
        L=1000;
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(globalStep(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    

    //then for maximization
    flagCont=1;
    Polygon pMax;
    ourArea=0;
    pArea=0;
    pArea2=0;//in this variable we store the area calculated by our algorithm

    initializeTime(allPoints->size());
    if(incremental(&pMax, allPoints, 1, 3, &ourArea) == -10)//4th argument either 3 or 1 (max or random edge selection)
        flagCont=0;
    pArea = abs(pMax.area());
    if(flagCont){
        L=1000;
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
        int threshold=1;
        L=10;
        L=1000;
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(globalStep(&pMin, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        maxScore=0;
    else
        maxScore = (double)pArea2/(double)chArea;

    //update info case vector with the new scores/bounds
    for(int i=0; i<infoCase4->size(); i++){
        if(infoCase4->at(i)->getSize() == allPoints->size()){
            infoCase4->at(i)->setMinScore(minScore);
            infoCase4->at(i)->setMinBound(minScore);
            infoCase4->at(i)->setMaxScore(maxScore);
            infoCase4->at(i)->setMaxBound(maxScore);
            break;
        }
    }
}