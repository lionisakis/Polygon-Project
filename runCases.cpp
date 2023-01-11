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

void findL_M(vector<double>* minAllTemp,vector<double>* maxAllTemp,vector<int>* mValues,double* LMin,int* mMin,double* LMax,int* mMax){
    double minAll=minAllTemp->at(0);
    int minPos=0;
    double maxAll=maxAllTemp->at(0);
    int maxPos=0;
    for(int i=1; i<minAllTemp->size(); i++){

        if(minAllTemp->at(i)<minAll){
            minAll=minAllTemp->at(i);
            minPos=i;
        }

        if(maxAllTemp->at(i)>maxAll){
            maxAll=maxAllTemp->at(i);
            maxPos=i;
        }
    }
    
    if(minPos<4){
        *LMin=1000;
        *mMin=mValues->at(minPos);
    }
    else if(minPos<8){
        *LMin=5000;
        *mMin=mValues->at(minPos-4);
    }
    else if(minPos<12){
        *LMin=10000;
        *mMin=mValues->at(minPos-8);
    }
    else if(minPos<16){
        *LMin=15000;
        *mMin=mValues->at(minPos-12);
    }

    if(maxPos<4){
        *LMax=1000;
        *mMax=mValues->at(maxPos);
    }
    else if(maxPos<8){
        *LMax=5000;
        *mMax=mValues->at(maxPos-4);
    }
    else if(maxPos<12){
        *LMax=10000;
        *mMax=mValues->at(maxPos-8);
    }
    else if(maxPos<16){
        *LMax=15000;
        *mMax=mValues->at(maxPos-12);
    }
}

void findL(vector<double>* minAllTemp,vector<double>* maxAllTemp,double* LMin,double* LMax){
    double minAll=minAllTemp->at(0);
    int minPos=0;
    double maxAll=maxAllTemp->at(0);
    int maxPos=0;
    for(int i=1; i<minAllTemp->size(); i++){
        if(minAllTemp->at(i)<minAll){
            minAll=minAllTemp->at(i);
            minPos=i;
        }

        if(maxAllTemp->at(i)>maxAll){
            maxAll=maxAllTemp->at(i);
            maxPos=i;
        }
    }
    
    if(minPos<4){
        *LMin=1000;
    }
    else if(minPos<8){
        *LMin=5000;
    }
    else if(minPos<12){
        *LMin=10000;
    }
    else if(minPos<16){
        *LMin=15000;
    }

    if(maxPos<4){
        *LMax=1000;
    }
    else if(maxPos<8){
        *LMax=5000;
    }
    else if(maxPos<12){
        *LMax=10000;
    }
    else if(maxPos<16){
        *LMax=15000;
    }
}

double runCase1Min(vector<Point>* allPoints, double chArea, int m, double L, double threshold){
    Polygon pMin;
    int finalRes=0, flagCont=1;
    double minScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    int res=5;
    while(res == 5){
        pMin.clear();
        if((res= subdivision(&pMin, allPoints, 2, L, &pArea2, allPoints->size(), chArea, 2, 1, m, &pArea)) == -10)//3rd argument from end either 2 or 1 (min or random edge selection)
                flagCont=0;
        
        if(res==5){
            cout <<"we cannot create a polygon so we change m" << endl;
            if(m>20) 
                m = m-10;
            else 
                m = 100;
        }
    }
    if(flagCont)
        finalRes = localSearch(&pMin, 2, threshold, 10, &pArea2, allPoints->size());

    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    return minScore;
        
}

double runCase1Max(vector<Point>* allPoints, double chArea, int m, double L, double threshold){
    Polygon pMax;
    int finalRes=0, flagCont=1;
    double maxScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    int res=5;
    while(res == 5){
        pMax.clear();
        if((res= subdivision(&pMax, allPoints, 1, L, &pArea2, allPoints->size(), chArea, 2, 1, m, &pArea)) == -10)//3rd argument from end either 2 or 1 (min or random edge selection)
                flagCont=0;
        
        if(res==5){
            cout <<"we cannot create a polygon so we change m" << endl;
            if(m>20) 
                m = m-10;
            else 
                m = 100;
        }
    }
    if(flagCont)
        finalRes = localSearch(&pMax, 1, threshold, 10, &pArea2, allPoints->size());

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
    double L=10000, L2=10,LMax;
    int m=100,mMax;
    double threshold=1;
    
    //preprocess 
    if(preprocess){
        //first find best parameters for subdivision, L and m
        vector<double> LValues{1000, 5000, 10000, 15000};
        vector<int> mValues{25, 50, 75, 100};
        vector<double> minAllTemp;
        vector<double> maxAllTemp;

        for(int i=0; i<LValues.size(); i++){
            for(int j=0; j<mValues.size(); j++){
                minAllTemp.push_back(runCase1Min(allPoints, chArea, mValues.at(j), LValues.at(i), threshold));
                maxAllTemp.push_back(runCase1Max(allPoints, chArea, mValues.at(j), LValues.at(i), threshold));
            }
        }

        findL_M(&minAllTemp,&maxAllTemp,&mValues,&L,&m,&LMax,&mMax);
    }
    
    initializeTime(allPoints->size());
    minScore = runCase1Min(allPoints, chArea, m, L, threshold);
    
    //then for maximization
    if(preprocess){
        L=LMax;
        m=mMax;
    }
    
    initializeTime(allPoints->size());
    maxScore = runCase1Max(allPoints, chArea, m, L, threshold);


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

double runCase2Min(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMin;
    int finalRes=0, flagCont=1;
    double minScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    int res=5;

    while(res == 5){
        pMin.clear();
        if((res=incremental(&pMin, allPoints, 1, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }

    pArea = abs(pMin.area());
    if(flagCont){
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
        finalRes = localSearch(&pMin, 2, threshold, 10, &pArea2, allPoints->size());
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;

    return minScore;
}

double runCase2Max(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMax;
    int finalRes=0, flagCont=1;
    double maxScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    
    int res=5;
    while(res == 5){
        pMax.clear();
        if((res=incremental(&pMax, allPoints, 1, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }

    pArea = abs(pMax.area());
    if(flagCont){
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
        finalRes = localSearch(&pMax, 1, threshold, 10, &pArea2, allPoints->size());
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        maxScore=0;
    else
        maxScore = (double)pArea2/(double)chArea;

    return maxScore; 
}

//incremental->localStep->localSearch
void runCase2(vector<Point>* allPoints, vector<outputInfo *>* infoCase2, double chArea, int preprocess){
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    //first for minimization
    double L=10000, L2=10,LMax;
    double threshold=1;
    
    //preprocess 
    if(preprocess){
        //first find best parameters for subdivision, L and m
        vector<double> LValues{1000, 5000, 10000, 15000};
        vector<double> minAllTemp;
        vector<double> maxAllTemp;

        for(int i=0; i<LValues.size(); i++){
            minAllTemp.push_back(runCase2Min(allPoints, chArea, LValues.at(i), threshold));
            maxAllTemp.push_back(runCase2Max(allPoints, chArea, LValues.at(i), threshold));
        }

        findL(&minAllTemp,&maxAllTemp,&L,&LMax);
    }
    
    initializeTime(allPoints->size());
    minScore = runCase2Min(allPoints, chArea, L, threshold);
    
    //then for maximization
    if(preprocess){
        L=LMax;
    }
    
    initializeTime(allPoints->size());
    maxScore = runCase2Max(allPoints, chArea,  L, threshold);


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

double runCase3Min(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMin;
    int finalRes=0, flagCont=1;
    double minScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm

    int res=5;
    while(res == 5){
        pMin.clear();
        if((res=convexHull(&pMin, allPoints, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }

    pArea = abs(pMin.area());

    if(flagCont){
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(globalStep(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }

    if(flagCont){
        int initialEnergy2 = minEnergy(allPoints->size(), pArea2, chArea);
        finalRes = localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy2, chArea);
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    
    return minScore;
}

double runCase3Max(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMax;
    int finalRes=0, flagCont=1;
    double maxScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    
    int res=5;
    while(res == 5){
        pMax.clear();
        if((res=convexHull(&pMax, allPoints, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }
    pArea = abs(pMax.area());
    
    if(flagCont){
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(globalStep(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }

    if(flagCont){
        int initialEnergy2 = maxEnergy(allPoints->size(), pArea2, chArea);
        finalRes = localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy2, chArea);
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        maxScore = 0;
    else
        maxScore = (double)pArea2/(double)chArea;

    return maxScore; 
}

//convexHull->globalStep->localStep
void runCase3(vector<Point>* allPoints, vector<outputInfo *>* infoCase3, double chArea, int preprocess){
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    //first for minimization
    double L=10000, L2=10,LMax;
    double threshold=1;
    
    //preprocess 
    if(preprocess){
        //first find best parameters for subdivision, L and m
        vector<double> LValues{1000, 5000, 10000, 15000};
        vector<int> mValues{25, 50, 75, 100};
        vector<double> minAllTemp;
        vector<double> maxAllTemp;

        for(int i=0; i<LValues.size(); i++){
            minAllTemp.push_back(runCase3Min(allPoints, chArea, LValues.at(i), threshold));
            maxAllTemp.push_back(runCase3Max(allPoints, chArea, LValues.at(i), threshold));
        }

        findL(&minAllTemp,&maxAllTemp,&L,&LMax);
    }
    
    initializeTime(allPoints->size());
    minScore = runCase3Min(allPoints, chArea, L, threshold);
    
    //then for maximization
    if(preprocess){
        L=LMax;
    }
    
    initializeTime(allPoints->size());
    maxScore = runCase3Max(allPoints, chArea, L, threshold);

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

double runCase4Min(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMin;
    int finalRes=0, flagCont=1;
    double minScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm

    int res=5;
    while(res == 5){
        pMin.clear();
        if((res=incremental(&pMin, allPoints, 1, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }

    pArea = abs(pMin.area());
    if(flagCont){
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
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
    
    return minScore;
}

double runCase4Max(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMax;
    int finalRes=0, flagCont=1;
    double maxScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm

   int res=5;
    while(res == 5){
        pMax.clear();
        if((res=incremental(&pMax, allPoints, 1, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }
    pArea = abs(pMax.area());
    if(flagCont){
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }
    if(flagCont){
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

    return maxScore; 
}

// incremental->localStep->globalStep
void runCase4(vector<Point>* allPoints, vector<outputInfo *>* infoCase4, double chArea, int preprocess){
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    //first for minimization
    double L=10000, L2=10,LMax;
    double threshold=1;
    
    //preprocess 
    if(preprocess){
        //first find best parameters for subdivision, L and m
        vector<double> LValues{1000, 5000, 10000, 15000};
        vector<int> mValues{25, 50, 75, 100};
        vector<double> minAllTemp;
        vector<double> maxAllTemp;

        for(int i=0; i<LValues.size(); i++){
            minAllTemp.push_back(runCase4Min(allPoints, chArea, LValues.at(i), threshold));
            maxAllTemp.push_back(runCase4Max(allPoints, chArea, LValues.at(i), threshold));
        }

        findL(&minAllTemp,&maxAllTemp,&L,&LMax);
    }
    
    initializeTime(allPoints->size());
    minScore = runCase4Min(allPoints, chArea, L, threshold);
    
    //then for maximization
    if(preprocess){
        L=LMax;
    }
    
    initializeTime(allPoints->size());
    maxScore = runCase4Max(allPoints, chArea, L, threshold);

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


double runCase5Min(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMin;
    int finalRes=0, flagCont=1;
    double minScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm

    int res=5;
    while(res == 5){
        pMin.clear();
        if((res=convexHull(&pMin, allPoints, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }

    pArea = abs(pMin.area());

    if(flagCont){
        int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }

    if(flagCont){
        finalRes = localSearch(&pMin, 2, threshold, 10, &pArea2, allPoints->size());
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        minScore = 1;
    else
        minScore = (double)pArea2/(double)chArea;
    
    return minScore;
}

double runCase5Max(vector<Point>* allPoints, double chArea, double L, double threshold){
    Polygon pMax;
    int finalRes=0, flagCont=1;
    double maxScore;
    double ourArea=0;
    int pArea=0, pArea2=0;//in this variable we store the area calculated by our algorithm
    
    int res=5;
    while(res == 5){
        pMax.clear();
        if((res=convexHull(&pMax, allPoints, 1, &ourArea)) == -10){//4th argument either 2 or 1 (min or random edge selection)
            flagCont=0;
        }
    }
    pArea = abs(pMax.area());
    
    if(flagCont){
        int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
        if(localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
            flagCont=0;
        }
    }

    if(flagCont){
        finalRes = localSearch(&pMax, 1, threshold, 10, &pArea2, allPoints->size());
    }
    if(pArea2 == 0)
        pArea2 = pArea;

    if(flagCont == 0 || finalRes == -10)
        maxScore = 0;
    else
        maxScore = (double)pArea2/(double)chArea;

    return maxScore; 
}

//convexHull->localStep->localSearch
void runCase5(vector<Point>* allPoints, vector<outputInfo *>* infoCase5, double chArea, int preprocess){
    double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
    double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
    //first for minimization
    double L=10000, L2=10,LMax;
    double threshold=1;
    
    //preprocess 
    if(preprocess){
        //first find best parameters for subdivision, L and m
        vector<double> LValues{1000, 5000, 10000, 15000};
        vector<int> mValues{25, 50, 75, 100};
        vector<double> minAllTemp;
        vector<double> maxAllTemp;

        for(int i=0; i<LValues.size(); i++){
            minAllTemp.push_back(runCase5Min(allPoints, chArea, LValues.at(i), threshold));
            maxAllTemp.push_back(runCase5Max(allPoints, chArea, LValues.at(i), threshold));
        }

        findL(&minAllTemp,&maxAllTemp,&L,&LMax);
    }
    
    initializeTime(allPoints->size());
    minScore = runCase5Min(allPoints, chArea, L, threshold);
    
    //then for maximization
    if(preprocess){
        L=LMax;
    }
    
    initializeTime(allPoints->size());
    maxScore = runCase5Max(allPoints, chArea, L, threshold);

    //update info case vector with the new scores/bounds
    for(int i=0; i<infoCase5->size(); i++){
        if(infoCase5->at(i)->getSize() == allPoints->size()){
            infoCase5->at(i)->setMinScore(minScore);
            infoCase5->at(i)->setMinBound(minScore);
            infoCase5->at(i)->setMaxScore(maxScore);
            infoCase5->at(i)->setMaxBound(maxScore);
            break;
        }
    }
}