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
//incremental->localstep->localSearch
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
        int finalres;
        int flagcont=1;
        if(incremental(&pMin, allPoints, 1, 2, &ourArea) == -10)//4th argument either 2 or 1 (min or random edge selection)
            flagcont=0;
        pArea = abs(pMin.area());
        if(flagcont){
            L=1000;
            int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
            if(localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
                flagcont=0;
            }
        }
        if(flagcont){
            int threshold=1;
            L=10;
            finalres = localSearch(&pMin, 2, threshold, L, &pArea2, allPoints->size());
        }
        if(pArea2 == 0)
            pArea2 = pArea;

        if(flagcont == 0 || finalres == -10)
            minScore = 1;
        else
            minScore = (double)pArea2/(double)chArea;
        

        //then for maximization
        flagcont=1;
        Polygon pMax;
        ourArea=0;
        pArea=0;
        pArea2=0;//in this variable we store the area calculated by our algorithm

        initializeTime(allPoints->size());
        if(incremental(&pMax, allPoints, 1, 3, &ourArea) == -10)//4th argument either 3 or 1 (max or random edge selection)
            flagcont=0;
        pArea = abs(pMax.area());
        if(flagcont){
            L=1000;
            int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
            if(localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
                flagcont=0;
            }
        }
        if(flagcont){
            int threshold=1;
            L=10;
            finalres = localSearch(&pMax, 1, threshold, L, &pArea2, allPoints->size());
        }
        if(pArea2 == 0)
            pArea2 = pArea;

        if(flagcont == 0 || finalres == -10)
            maxScore=0;
        else
            maxScore = (double)pArea2/(double)chArea;

        //update infocase vector with the new scores/bounds
        for(int i=0; i<infoCase2->size(); i++){
            if(infoCase2->at(i)->getSize() == allPoints->size()){
                infoCase2->at(i)->setminScore(minScore);
                infoCase2->at(i)->setminBound(minScore);
                infoCase2->at(i)->setmaxScore(maxScore);
                infoCase2->at(i)->setmaxBound(maxScore);
                break;
            }
        }
}

//convexHull->globalStep->localstep
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
        int finalres;
        int flagcont=1;
        if(convexHull(&pMin, allPoints, 2, &ourArea) == -10)//3rd argument either 2 or 1 (min or random edge selection)
            flagcont=0;
        pArea = abs(pMin.area());

        if(flagcont){
            L=1000;
            int initialEnergy = minEnergy(allPoints->size(), pArea, chArea);
            if(globalStep(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
                flagcont=0;
            }
        }

        if(flagcont){
            L=1000;
            int initialEnergy2 = minEnergy(allPoints->size(), pArea2, chArea);
            finalres = localMinimum(&pMin, 2, L, &pArea2, allPoints->size(), initialEnergy2, chArea);
        }
        if(pArea2 == 0)
            pArea2 = pArea;

        if(flagcont == 0 || finalres == -10)
            minScore = 1;
        else
            minScore = (double)pArea2/(double)chArea;
        

        //then for maximization
        flagcont=1;
        Polygon pMax;
        ourArea=0;
        pArea=0;
        pArea2=0;//in this variable we store the area calculated by our algorithm

        initializeTime(allPoints->size());
        if(convexHull(&pMax, allPoints, 3, &ourArea) == -10)//3rd argument either 3 or 1 (max or random edge selection)
            flagcont=0;
        pArea = abs(pMax.area());
        
        if(flagcont){
            L=1000;
            int initialEnergy = maxEnergy(allPoints->size(), pArea, chArea);
            if(globalStep(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy, chArea)== -10){
                flagcont=0;
            }
        }

        if(flagcont){
            L=1000;
            int initialEnergy2 = maxEnergy(allPoints->size(), pArea2, chArea);
            finalres = localMinimum(&pMax, 1, L, &pArea2, allPoints->size(), initialEnergy2, chArea);
        }
        if(pArea2 == 0)
            pArea2 = pArea;

        if(flagcont == 0 || finalres == -10)
            maxScore = 0;
        else
            maxScore = (double)pArea2/(double)chArea;

        //update infocase vector with the new scores/bounds
        for(int i=0; i<infoCase3->size(); i++){
            if(infoCase3->at(i)->getSize() == allPoints->size()){
                infoCase3->at(i)->setminScore(minScore);
                infoCase3->at(i)->setminBound(minScore);
                infoCase3->at(i)->setmaxScore(maxScore);
                infoCase3->at(i)->setmaxBound(maxScore);
                break;
            }
        }
}


// // put all the correct arguments such as L,m,edge_selection etc
// void runCase1(vector<Point>* allPoints){
//     //start timer 
//     auto start = chrono::steady_clock::now();

//     //code
//     Polygon p;
//     double ourArea=0;//in this variable we store the area calculated by our algorithm
//     int pArea=0, pArea2=0;//initial and final area respectively
//     double ratio=0, ratio2=0;//initial and final ratio respectively


//     if (algo==1){

//         //this has to be changed
//         if(algo2 == 2)
//             convexHull(&p, &allPoints, edge, &ourArea);
//         else if (algo2 == 1)
//             incremental(&p,&allPoints,init,edge,&ourArea);
        

//         pArea = abs(p.area());//in this variable we store the area calculated by cgal function for the greedy solution
//         ratio = ((double)pArea/(double)chArea);//ratio for the greedy solution
        
//         if(max)
//             localSearch(&p, 1, threshold,  L, &pArea2,allPoints.size());
//         else if(min)
//             localSearch(&p, 2, threshold,  L, &pArea2,allPoints.size());
        
//     }
//     else if (algo==2){
//         int initialEnergy;
//         if (annealing!=3){
//             if(algo2 == 2)
//                 convexHull(&p, &allPoints, edge, &ourArea);
//             else if (algo2 == 1)
//                 incremental(&p,&allPoints,init,edge,&ourArea);

//         pArea = abs(p.area());//in this variable we store the area calculated by cgal function for the greedy solution
//         ratio = ((double)pArea/(double)chArea);//ratio for the greedy solution
//         if(max){
//                 initialEnergy = maxEnergy(allPoints.size(), pArea, chArea);
//                 simulated_annealing(&p, 1, L, &pArea2, allPoints.size(), annealing, initialEnergy, chArea);
//             }
//         else if(min){
//                 initialEnergy = minEnergy(allPoints.size(), pArea, chArea);
//                 simulated_annealing(&p, 2, L, &pArea2, allPoints.size(), annealing, initialEnergy, chArea);
//             }
//         }
//         else{

//             if(max){
//                 subdivision(&p, &allPoints, 1, L, &pArea2, allPoints.size(), chArea, algo2, edge, m, &pArea);
//             }
//             else if (min){
//                 subdivision(&p, &allPoints, 2, L, &pArea2, allPoints.size(), chArea, algo2, edge, m, &pArea);
//             }
//             ratio = ((double)pArea/(double)chArea);
//         }
            
//     }

//     //area and ratio for the final polygon aftetr optimization
//     if(pArea2 == 0)
//         pArea2 = pArea;
//     ratio2 = ((double)pArea2/(double)chArea);
    
//     cout << "simple = " << p.is_simple() << endl;

//     auto end = chrono::steady_clock::now();
// }
