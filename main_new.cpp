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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Segment_2<K> Segment;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;

using namespace std;


int main(int argc, char* argv[]){ 

    unsigned int tmp = (unsigned) time(NULL);
    srand(tmp);

    if(argc != 13 ){//check if the #arguments is correct
        cout << "not enough arguments were given" ;
        return 0;
    }
    string input;//store name of input file
    input.assign(argv[2]);
    cout << "name of input file: " << input << "\n";

    string output;
    output.assign(argv[4]);
    cout << "name of output file: " << output << "\n";

    int algo;//1: local_search , 2: simulated_annealing
    int L;//L parameter, must be lower than 10 if we have local search, otherwise it shows maximum iterations
    int max, min;//only on of these will be found in command line arguments


    //find type of algorithm
    std::string typeOfAlgo(argv[6]);
    cout << typeOfAlgo << "\n";
    if(typeOfAlgo == "local_search")
        algo=1;
    else if (typeOfAlgo == "simulated_annealing")
        algo=2;

    //save L parameter
    L = atoi(argv[8]);

    std::string type(argv[9]);
    //find if we want to minimize or maximize the area
    if(type == "-max"){
        max = 1;
        min =0 ;
    }
    else if(type == "-min"){
        max=0; 
        min=1;
    }

    double threshold;//only for local search
    if(algo == 1){
        threshold = stod(argv[12]);
    }

    int annealing; // 1:local, 2:global, 3:subdivision
    if(algo==2){
        std::string typeOfAnnealing(argv[12]);
        if(typeOfAnnealing == "local")
            annealing=1;
        else if (typeOfAnnealing == "global")
            annealing=2;
        else if (typeOfAnnealing == "subdivision")
            annealing=3;
    }

    //read contents of input file, ignoring first line with #
    vector<Point> allPoints;//vector of type Point_2(from CGAL) to store coordinates of points given
    fstream in;
    in.open(input, ios::in);
    if(!in){
        cout <<"error while opening file" << endl;
    }
    int totalPoints=0;//variable to store the total amount of 2D points given in input file
    int count=0;
    double chArea;//area of convex hull, written in second line of input line
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
            int wordcounter=0;
            while (token2) {
                wordcounter++;
                if(wordcounter==6){
                    chArea=stod(token2);
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
        allPoints.push_back(temp);
        cout << temp << endl;
    }
    int i=0;
    cout <<"total points = " << allPoints.size() << endl;
    // string check=".instance";
    // for(i= 8; i>=0; i--){
    //     if(check[i]!=input[input.size()-9+i])
    //         break;
    // }
    // if (i==-1)
    //     allPoints.pop_back();

    in.close();



    //start timer 
    auto start = chrono::steady_clock::now();

    //code
    Polygon p;
    double ourArea=0;//in this variable we store the area calculated by our algorithm
    int pArea, pArea2;//initial and final area respectively
    double ratio, ratio2;//initial and final ratio respectively


    if (algo==1){

        //this has to be changed
        if(max){
            convexHull(&p, &allPoints, 2, &ourArea);//the algorithm selection is based on the paper from eclass
            pArea = abs(p.area());//in this variable we store the area calculated by cgal function
            ratio = ((double)pArea/(double)chArea);
        }
        else if (min){/
            int init = 1 + (rand()%3);
            incremental(&p,&allPoints,init,3,&ourArea);
            pArea = abs(p.area());//in this variable we store the area calculated by cgal function
            ratio = ((double)pArea/(double)chArea);
        }

        if(max)
            localSearch(&p, 1, threshold,  L, &pArea2);
        else if(min)
            localSearch(&p, 2, threshold,  L, &pArea2);
    }
    else if (algo==2){
        
    }


    
    cout << "simple = " << p.is_simple() << endl;

    auto end = chrono::steady_clock::now();



    //output

    //create file where output is to be written
    ofstream outfile(output);

    outfile << "Polygonization" << endl;


    for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
        outfile  << *vi << endl;
    outfile << endl;
    for (EdgeIterator ei = p.edges_begin(); ei != p.edges_end(); ++ei)
        outfile << *ei << endl;


    outfile << "Algorithm: <" << argv[6] <<">_<" << argv[9] << ">" << endl;
    outfile << "Our Initial Area: " << ourArea << endl;
    outfile << "Initial Area: " << pArea << endl;
    outfile << "Final Area: " << pArea2 << endl;
    outfile << "Ratio Initial: " << ratio << endl;
    outfile << "Ratio Final: " << ratio2 << endl;
    outfile << "Construction time in miliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    outfile.close();


    return 0;
}
