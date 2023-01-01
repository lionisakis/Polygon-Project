#include <iostream>
#include <vector>
#include <fstream>


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
        double setminScore(double sc);
        double setmaxScore(double sc);
        double setmaxBound(double sc);
        double setminBound(double sc);
        void printInfo(ofstream* );
};