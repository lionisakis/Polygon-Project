//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Polygon_2.h>
//#include <list>
//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Point_2<K> Point;
//typedef CGAL::Polygon_2<K> Polygon_2;
//typedef Polygon_2::Vertex_iterator VertexIterator;
//typedef Polygon_2::Edge_const_iterator EdgeIterator;
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

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
    fstream in;
    in.open(input, ios::in);
    if(!in){
        cout <<"error while opening file" << endl;
    }
    int totalPoints=0;//variable to store the total amount of 2D points given in input file

    while(!in.eof()){
        string text;
        getline(in, text);

        if(text == "-1")
            break;

        if(text[0] == '#')
            continue;
        else
            totalPoints++;
        
        cout << text << endl;//all numbers are separated by \t(tab) so they are in positions 0, 4, 8 of the string
        
    }
    in.close();
    return 0;
}