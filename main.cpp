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
#include <chrono>
#include <time.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include  <CGAL/Polygon_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Segment_2<K> Segment;

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

    while(!in.eof()){
        string text;
        getline(in, text);

        if(text == "-1")
            break;

        if(text[0] == '#')
            continue;
        
        totalPoints++;
        int x, y;

        x=stoi(&text[4]);
        y=stoi(&text[8]);
        Point temp(x, y);
        allPoints.push_back(temp);
        cout <<"aaa" << endl;
        cout << text << endl;//all numbers are separated by \t(tab) so they are in positions 0, 4, 8 of the string
    }
    in.close();
    for(int i=0; i<totalPoints; i++)
        cout << allPoints[i] << endl;


    // Timer clock; // Timer<milliseconds, steady_clock>
    // clock.tick();
    

    // //code
    // Polygon p;
    // clock.tock();



    //output
    cout << "Polygonization" << endl;

    // for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
    //     std::cout << "vertex " << n++ << " = " << *vi << std::endl;
    // std::cout << std::endl;
    // n=0;
    // for (EdgeIterator ei = p.edges_begin(); ei != p.edges_end(); ++ei)
    //     std::cout << "edge " << n++ << " = " << *ei << std::endl;

    cout << "Algorithm: <" << argv[6] <<">_<" << argv[8] << ">" << endl;
    cout <<"Area: " << "replace with variable that stores area of polygon" << endl;
    cout << "Ration: " << "replace with variable" << endl;
    //cout << "Construction time in miliseconds: " << clock.duration().count() << " ms\n";

    return 0;
}