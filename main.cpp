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

#include  "mainFunctions.hpp"


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Segment_2<K> Segment;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;

using namespace std;


int main(int argc, char* argv[]){ 

    if(argc != 11 && argc != 9){//check if the #arguments is correct
        cout << "not enough arguments were given" ;
        return 0;
    }
    string input;//store name of input file
    input.assign(argv[2]);
    cout << "name of input file: " << input << "\n";

    string output;
    output.assign(argv[4]);
    cout << "name of output file: " << output << "\n";

    int algo;//1: incremental , 2: convex_hull
    int edge;//1: random selection, 2:min_area, 3: max_area
    int init;//1:x_decreasing(1a), 2:x_increasing(1b), 3:y_decreasing(2a), 4:y_increasing(2b)

    //find type of algorithm
    std::string typeOfAlgo(argv[6]);
    cout << typeOfAlgo << "\n";
    if(typeOfAlgo == "incremental")
        algo=1;
    else if (typeOfAlgo == "convex_hull")
        algo=2;

    //find type of edge selector
    if(atoi(argv[8]) == 1)
        edge=1;
    else if (atoi(argv[8]) == 2)
        edge=2;
    else if (atoi(argv[8]) == 3)
        edge=3;

    //find type of initialization only if algo=1(incremental)
    if(algo == 1){
        std::string typeOfInit(argv[10]);
        if(typeOfInit == "1a")
            init=1;
        else if (typeOfInit == "1b")
            init=2;
        else if (typeOfInit == "2a")
            init=3;
        else if (typeOfInit == "2b")
            init=4;
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
        char delim2[2] = "\t";
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
    }
    in.close();

    //start timer 
    auto start = chrono::steady_clock::now();

    //code
    Polygon p;
    double ourArea=0;//in this variable we store the area calculated by our algorithm
    if (algo==1){
        incremental(&p,&allPoints,init,edge,&ourArea);
    }
    else
        convexHull(&p, &allPoints, edge, &ourArea);
    int pArea = p.area();//in this variable we store the area calculated by cgal function
    double ratio = ((double)pArea/(double)chArea);
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
    if(argc==11){
        outfile << "Algorithm: <" << argv[6] <<">_<" << argv[8] << ">_<" << argv[10] << ">" << endl;
        outfile << "Our Area: " << ourArea << endl;
    }
    else if(argc==9){
        outfile << "Algorithm: <" << argv[6] <<">_<" << argv[8] <<  ">" << endl;
        outfile << "Our Area: " << ourArea << endl;
    }
    outfile << "Area: " << pArea << endl;
    outfile << "Ratio: " << ratio << endl;
    outfile << "Construction time in miliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    outfile.close();


    return 0;
}
