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

double runCase1Min(vector<Point>* allPoints, double chArea, int m, double L, int L2, double threshold){
    Polygon pMin;
    int finalRes=0, flagCont=1;
    double minScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm

    if(subdivision(&pMin, allPoints, 2, L, &pArea2, allPoints->size(), chArea, 2, 1, m, &pArea) == -10)//3rd argument from end either 2 or 1 (min or random edge selection)
            flagCont=0;

    if(flagCont)
        finalRes = localSearch(&pMin, 2, threshold, L2, &pArea2, allPoints->size());

    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    return minScore;
        
}

double runCase1Max(vector<Point>* allPoints, double chArea, int m, double L, int L2, double threshold){
    Polygon pMax;
    int finalRes=0, flagCont=1;
    double maxScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    
    if(subdivision(&pMax, allPoints, 1, L, &pArea2, allPoints->size(), chArea, 2, 3, m, &pArea) == -10)//3rd argument from end either 3 or 1 (max or random edge selection)
        flagCont=0;

    if(flagCont)
        finalRes = localSearch(&pMax, 1, threshold, L2, &pArea2, allPoints->size());

    if(flagCont == 0 || finalRes == -10)
        maxScore=0;
    else
        maxScore = (double)pArea2/(double)chArea;
    return maxScore;
        
}

// subdivision -> localSearch
void runCase1(vector<Point>* allPoints, vector<outputInfo *>* infoCase1, double chArea, int preprocess){
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    //first for minimization
    double L=10000, L2=10;
    int m=100;
    double threshold=0.1;
    double Lmax,mMax;
    
    //preprocess 
    if(preprocess){
        //first find best parameters for subdivision, L and m
        vector<double> Lvalues{1000, 5000, 10000, 15000};
        vector<int> mvalues{25, 50, 75, 100};
        vector<double> minAllTemp;
        vector<double> maxAllTemp;
        for(int i=0; i<Lvalues.size(); i++){
            for(int j=0; j<mvalues.size(); j++){
                minAllTemp.push_back(runCase1Min(allPoints, chArea, mvalues.at(j), Lvalues.at(i), L2, threshold));
                maxAllTemp.push_back(runCase1Max(allPoints, chArea, mvalues.at(j), Lvalues.at(i), L2, threshold));
            }
        }

        double minAll=minAllTemp.at(0);
        int minPos=0;
        double maxAll=maxAllTemp.at(0);
        int maxPos=0;
        for(int i=1; i<minAllTemp.size(); i++){

            if(minAllTemp.at(i)<minAll){
                minAll=minAllTemp.at(i);
                minPos=i;
            }

            if(maxAllTemp.at(i)>maxAll){
                maxAll=maxAllTemp.at(i);
                maxPos=i;
            }
        }
        
        if(minPos<4){
            L=1000;
            m=mvalues.at(minPos);
        }
        else if(minPos<8){
            L=5000;
            m=mvalues.at(minPos-4);
        }
        else if(minPos<12){
            L=10000;
            m=mvalues.at(minPos-8);
        }
        else if(minPos<16){
            L=15000;
            m=mvalues.at(minPos-12);
        }
        if(maxPos<4){
            Lmax=1000;
            mMax=mvalues.at(maxPos);
        }
        else if(maxPos<8){
            Lmax=5000;
            mMax=mvalues.at(maxPos-4);
        }
        else if(maxPos<12){
            Lmax=10000;
            mMax=mvalues.at(maxPos-8);
        }
        else if(maxPos<16){
            Lmax=15000;
            mMax=mvalues.at(maxPos-12);
        }
    }
    
    initializeTime(allPoints->size());
    minScore = runCase1Min(allPoints, chArea, m, L, L2, threshold);
    
    if(preprocess){
        L=Lmax;
        m=mMax;
    }
    
    //then for maximization
    initializeTime(allPoints->size());
    maxScore = runCase1Max(allPoints, chArea, m, L, L2, threshold);


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
void runCase2(vector<Point>* allPoints, vector<outputInfo *>* infoCase2, double chArea, int preprocess){
    double L;
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
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
        double threshold=1;
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
        double threshold=1;
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
void runCase3(vector<Point>* allPoints, vector<outputInfo *>* infoCase3, double chArea, int preprocess){
    double L;
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff

    //first for minimization
    Polygon pMin;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    initializeTime(allPoints->size());
    int finalRes;
    int flagCont=1;
    if(convexHull(&pMin, allPoints, 1, &ourArea) == -10)//3rd argument either 2 or 1 (min or random edge selection)
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
    if(convexHull(&pMax, allPoints, 1, &ourArea) == -10)//3rd argument either 3 or 1 (max or random edge selection)
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
void runCase4(vector<Point>* allPoints, vector<outputInfo *>* infoCase4, double chArea, int preprocess){
    double L;
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff

    //first for minimization
    Polygon pMin;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    initializeTime(allPoints->size());
    int flagCont=1;
    if(incremental(&pMin, allPoints, 1, 1, &ourArea) == -10)//4th argument either 2 or 1 (min or random edge selection)
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
        int initialEnergy2 = minEnergy(allPoints->size(), pArea2, chArea);
        if(globalStep(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy2, chArea)== -10){
            flagCont=0;
        }
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0)
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
    if(incremental(&pMax, allPoints, 1, 1, &ourArea) == -10)//4th argument either 3 or 1 (max or random edge selection)
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
        double threshold=1;
        L=10;
        L=1000;
        int initialEnergy2 = maxEnergy(allPoints->size(), pArea2, chArea);
        if(globalStep(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy2, chArea)== -10){
            flagCont=0;
        }
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0)
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