#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <sstream>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point_2;

int main()
{
    Point_2 p, q, r;
    {
        std::istringstream input("0 0.3 1 0.6 2 0.9");
        input >> p >> q >> r;
        std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not"
        " collinear\n");
    }
    {
        std::istringstream input("0 0.4 1 0.8 2 1.2");
        input >> p >> q >> r;
        std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not"
        " collinear\n");
    }
    {
        Point_2 p(0,0), q(1, 1), r(2, 2);
        std::cout << (CGAL::collinear(p,q,r) ? "collinear\n" : "not"
        "collinear\n");
    }
    return 0;
}