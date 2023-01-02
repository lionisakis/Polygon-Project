#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <experimental/filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h> 
#include <netdb.h>
#include <unistd.h>
#include <ctype.h> 
#include <dirent.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include  <CGAL/Polygon_2.h>
#include <sstream>
#include <cstring>
#include <cstdlib>

#include  "incremental.hpp"
#include "convexHull.hpp"
#include "localSearch.hpp"
#include "simulatedAnnealing.hpp"
#include "outputInfo.hpp"
#include "timeManager.hpp"
#include "genericUtil.hpp"

#define CASES 2

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Segment_2<K> Segment;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;

using namespace std;


void swap(outputInfo** xp, outputInfo** yp){
    outputInfo* temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// bubble sort, to sort the vectors with the statistics with increasing order of file size
//must be called for each vector of each case we run
void bubbleSort(vector<outputInfo*> arr, int n){
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr.at(j)->getSize() > arr.at(j+1)->getSize())
                swap(&arr.at(j), &arr.at(j+1));
}


void readFile(string input,vector<Point>* allPoints, double* chArea){
    //read contents of input file, ignoring first line with #
    fstream in;
    cout <<"name of file = " << input << endl;
    in.open(input, ios::in);
    if(!in){
        cout <<"error while opening file" << endl;
    }
    int totalPoints=0;//variable to store the total amount of 2D points given in input file
    int count=0;
    while(!in.eof()){
        string text;
        getline(in, text);
        if(text == "-1")
            break;
        if(text[0] == '#' && count==1){
            char delim[2] = "\"";
            char tmp[text.size() + 1];
            strcpy(tmp, text.c_str());
            char *token2 = strtok(tmp,delim);
            int wordCounter=0;
            while (token2) {
                wordCounter++;
                if(wordCounter==6){
                    *chArea=stod(token2);
                }
                token2 = strtok(NULL,delim);
            }
            count++;
            continue;
        }
        else if(text[0] == '#' && count==0){
            count++;
            continue;
        }
        
        totalPoints++;
        int x, y;
        //all numbers are separated by \t(tab) so i use strtok to get the coordinates of each point
        char delim2[3] = " \t";
        char tmp[text.size() + 1];
        strcpy(tmp, text.c_str());
        if(strlen(tmp) <= 2){
            continue;
        }
        char *token = strtok(tmp,delim2);
        int counter=0;
        while (token) {
            if(counter==1){
                x = stoi(token);
            }
            else if(counter==2){
                y = stoi(token);
            }
            token = strtok(NULL,delim2);
            counter++;
        }
        Point temp(x, y);
        allPoints->push_back(temp);
    }
    cout <<"total points = " << allPoints->size() << endl;

    in.close();
}
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

        for(int i=0; i<infoCase2->size(); i++){
            if(infoCase2->at(i)->getSize() == allPoints->size()){
                infoCase2->at(i)->setminScore(minScore);
                infoCase2->at(i)->setmaxScore(maxScore);
                infoCase2->at(i)->setmaxBound(maxScore);
                infoCase2->at(i)->setminBound(minScore);
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

int readFolder(string path,ofstream* outfile, int preprocessor){
    DIR *dir=opendir( path.c_str() );
    vector<outputInfo*> infoCase1;//for each case(combination of algorithms we have a separate vector for its ratio statistics)
    vector<outputInfo*> infoCase2;
    int totalSizes=0;//how many different totalPoint sizes we have
    double L;
    struct dirent *theDir;
    // try to open the path
    if ( dir  == NULL ){
        cout<<"Not a dir"<<endl;
        return -1;
    }

    // it is a folder
    string that("/");
    string newPath= path+that;

    int files=0;

    // read the folder
    while ( ( theDir=readdir(dir) ) != NULL ){ 
        // if the folder name is . or .. then do nothing
            if(strcmp(theDir->d_name,".")==0||strcmp(theDir->d_name,"..")==0)
                continue;
            string temp(theDir->d_name);
            // for all the files
            vector<Point> allPoints;
            double chArea;
            readFile(newPath+temp, &allPoints, &chArea);
            *outfile << allPoints.size() << "\t\t";
            double minScore=1;//is initializes to 1 in case the algorithm exceeds cutoff
            double maxScore=0;//is initializes to 0 in case the algorithm exceeds cutoff
            double minBound;
            double maxBound;

            //run case 1
            int flag=0;
            for(int i=0; i<infoCase1.size(); i++){
                if(infoCase1.at(i)->getSize() == allPoints.size()){
                    flag=1;
                    break;
                }
            }
            //if flag=0 then a new file size was found so have to add a new element to all vectors of type <outputInfo*>
            if(!flag){
                outputInfo*  newSize = new outputInfo(allPoints.size());
                infoCase1.push_back(newSize);
                infoCase2.push_back(newSize);
                totalSizes++;
            }

            
            

            //run case1

            //auta vgalta tha mpoun mesa stin sinartisi
            initializeTime(allPoints.size());
            // convexHull(&p, &allPoints, 1, &ourArea);
            
            //after the algorithm has finished we updates the scores for the specific size
            for(int i=0; i<infoCase1.size(); i++){
                if(infoCase1.at(i)->getSize() == allPoints.size()){
                    infoCase1.at(i)->setminScore(minScore);
                    infoCase1.at(i)->setmaxScore(maxScore);
                    infoCase1.at(i)->setmaxBound(maxScore);
                    infoCase1.at(i)->setminBound(minScore);
                    break;
                }
            }

            //run case2
            runCase2(&allPoints, &infoCase2, chArea);
        
    
    }
    //aftetr all cases are implemented for all files we print statistics
    for(int i=0; i<totalSizes; i++){
        infoCase1.at(i)->printInfo(outfile);
        *outfile << "||"<<endl;
        infoCase2.at(i)->printInfo(outfile);
        *outfile << "||"<<endl;
        //then printinfo of next case
    }

    //delete all vectors of type <outputInfo*>
    for (int i=0;i<infoCase1.size();i--){
        delete infoCase1.at(i);
        delete infoCase2.at(i);
    }
    return 0;
    
}



int main(int argc, char* argv[]){ 

    if (argc!=7){
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }

    string folderInput;
    if (strcmp(argv[1],"-i")==0){
        string temp(argv[2]);
        folderInput=temp;
    }
    else if(strcmp(argv[3],"-i")==0){
        string temp(argv[4]);
        folderInput=temp;
    }
    else if(strcmp(argv[5],"-i")==0){
        string temp(argv[6]);
        folderInput=temp;
    }
    else{
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }

    string fileOutput;
    if (strcmp(argv[1],"-o")==0){
        string temp(argv[2]);
        fileOutput=temp;
    }
    else if(strcmp(argv[3],"-o")==0){
        string temp(argv[4]);
        fileOutput=temp;
    }
    else if(strcmp(argv[5],"-o")==0){
        string temp(argv[6]);
        fileOutput=temp;
    }
    else{
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }

    int preprocess;
    if (strcmp(argv[1],"-preprocess")==0){
        preprocess=atoi(argv[2]);
    }
    else if(strcmp(argv[3],"-preprocess")==0){
        preprocess=atoi(argv[4]);
    }
    else if(strcmp(argv[5],"-preprocess")==0){
        preprocess=atoi(argv[6]);
    }
    else{
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }
    cout<< folderInput<<endl;
    cout << fileOutput<<endl;
    cout<< preprocess<<endl;

    unsigned int tmp = (unsigned) time(NULL);
    srand(tmp);
    ofstream outfile(fileOutput);
    outfile <<"\t\t";
    for (int i=0;i<CASES;i++)
        outfile <<"||\t\t\t\t\tAlgorithm\t"<<i+1<<"\t\t\t\t\t";
    outfile<<"||"<<endl;
    outfile <<"Size \t";
    for (int i=0;i<CASES;i++)
        outfile <<"||\t"  <<"min Score \t"<<"Max Score\t" <<"Min Bound\t"<< "Max Bound\t";
    outfile<<"||"<<endl;

    readFolder(folderInput, &outfile, preprocess);

    outfile.close();


    return 0;
}
