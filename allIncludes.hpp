#pragma once

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



#define CASES 3

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Segment_2<K> Segment;
typedef Polygon::Vertex_iterator VertexIterator;
typedef Polygon::Edge_const_iterator EdgeIterator;

using namespace std;
