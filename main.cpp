#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include  <CGAL/Polygon_2.h>
#include <sstream>
#include <cstring>
#include <cstdlib>

#include  "mainFunctions.hpp"
#include  "utility.hpp"
#include  "mainFunctions2.hpp"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Segment_2<K> Segment;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;

using namespace std;

// void readFile(string input,vector<Point>* allPoints){
//     //read contents of input file, ignoring first line with #
//     fstream in;
//     in.open(input, ios::in);
//     if(!in){
//         cout <<"error while opening file" << endl;
//     }
//     int totalPoints=0;//variable to store the total amount of 2D points given in input file
//     int count=0;
//     double chArea;//area of convex hull, written in second line of input line
//     while(!in.eof()){
//         string text;
//         getline(in, text);
//         if(text == "-1")
//             break;
//         if(text[0] == '#' && count==1){
//             char delim[2] = "\"";
//             char tmp[text.size() + 1];
//             strcpy(tmp, text.c_str());
//             char *token2 = strtok(tmp,delim);
//             int wordCounter=0;
//             while (token2) {
//                 wordCounter++;
//                 if(wordCounter==6){
//                     chArea=stod(token2);
//                 }
//                 token2 = strtok(NULL,delim);
//             }
//             count++;
//             continue;
//         }
//         else if(text[0] == '#' && count==0){
//             count++;
//             continue;
//         }
        
//         totalPoints++;
//         int x, y;
//         //all numbers are separated by \t(tab) so i use strtok to get the coordinates of each point
//         char delim2[3] = " \t";
//         char tmp[text.size() + 1];
//         strcpy(tmp, text.c_str());
//         char *token = strtok(tmp,delim2);
//         int counter=0;
//         while (token) {
//             if(counter==1){
//                 x = stoi(token);
//             }
//             else if(counter==2){
//                 y = stoi(token);
//             }
//             token = strtok(NULL,delim2);
//             counter++;
//         }
//         Point temp(x, y);
//         allPoints->push_back(temp);
//     }
//     int i=0;
//     string check=".instance";
//     for(i= 8; i>=0; i--){
//         if(check[i]!=input[input.size()-9+i])
//             break;
//     }
//     if (i==-1){
//         allPoints->pop_back();
//     }
//     cout <<"total points = " << allPoints.size() << endl;

//     in.close();
// }

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

// void makeOutputRunCase(ofstream outfile,double minScore,double maxScore,double minBound,double maxBound){
//     //create file where output is to be written
//     outfile << "||\t" << minScore <<"\t"<< maxScore <<"\t"<< minBound<<"\t"<< maxBound<<"\t";
// }

#define CASES 2

int main(int argc, char* argv[]){ 

    unsigned int tmp = (unsigned) time(NULL);
    srand(tmp);
    string output="./output.txt"
    ofstream outfile(output);
    outfile <<"\t";
    for (int i=0;i<CASES;i++)
        outfile <<"||\t\tAlgorithm\t"<<i+1<<"\t\t";
    outfile<<endl;
    outfile <<"Size \t";
    for (int i=0;i<CASES;i++)
        outfile <<"||\t"  <<"min Score \t"<<"Max Score\t" <<"Min Bound\t"<< "Max Bound\t";
    outfile<<endl;
    
    

    
    outfile.close();


    return 0;
}