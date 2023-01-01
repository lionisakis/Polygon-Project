#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>


class outputInfo{
    private:
        int size;
        double minScore;
        double maxScore;
        double minBound;
        double maxBound;

    public:
        outputInfo(int);
        const double getSize();
        const double getminScore();
        const double getmaxScore();
        const double getmaxBound();
        const double getminBound();
        void setminScore(double sc);
        void setmaxScore(double sc);
        void setmaxBound(double sc);
        void setminBound(double sc);
        void printInfo(std::ofstream* );
};