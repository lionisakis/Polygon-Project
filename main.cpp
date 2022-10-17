//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Polygon_2.h>
//#include <list>
//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <unistd.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include  <CGAL/Polygon_2.h>
#include <sstream>

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

    //read contents of input file, ignoring lines that start with "#"
    vector<Point> allPoints;//vector of type Point_2(from CGAL) to store coordinates of points given
    fstream in;
    in.open(input, ios::in);
    if(!in){
        cout <<"error while opening file" << endl;
    }
    int totalPoints=0;//variable to store the total amount of 2D points given in input file
    int count=0;
    int chArea=0;//area of convex hull 
    while(!in.eof()){
        string text;
        getline(in, text);

        if(text == "-1")
            break;
        if(text[0] == '#' && count==1){
            stringstream ss;
            ss << text;
            string temp;
            int found;
            while (!ss.eof()) {
                ss >> temp;
                if(temp.size() > 3){
                    temp.erase(0,1);
                    temp.erase(temp.size()-1);
                    temp.erase(temp.size()-1);
                }
                if (stringstream(temp) >> found){
                    chArea=found;
                }
                temp = "";
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
        //all numbers are separated by \t(tab) so they are in positions 0, 4, 8 of the string
        x=stoi(&text[4]);
        y=stoi(&text[8]);
        Point temp(x, y);
        allPoints.push_back(temp);
    }
    in.close();


    auto start = chrono::steady_clock::now();

    //code
    Polygon p;

    // TODO:: add more if
    cout<<"WHAT1"<<endl;
    if (algo==1)
        incremental(&p,&allPoints,init,edge);
    cout<<"WHAT2"<<endl;

    auto end = chrono::steady_clock::now();



    //output

    //create file where output is to be written
    ofstream outfile(output);

    outfile << "Polygonization" << endl;
    int n=0;
    for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
        outfile << "vertex " << n++ << " = " << *vi << endl;
    outfile << endl;
    n=0;
    for (EdgeIterator ei = p.edges_begin(); ei != p.edges_end(); ++ei)
        outfile << "edge " << n++ << " = " << *ei << endl;

    outfile << "Algorithm: <" << argv[6] <<">_<" << argv[8] << ">_<" << argv[10] << ">" << endl;
    outfile << "Area: " << "replace with variable that stores area of polygon" << endl;
    outfile << "Ration: " << "replace with variable" << endl;
    outfile << "Construction time in miliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
    outfile.close();


    return 0;
}
