#pragma once
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
        const double getMinScore();
        const double getMaxScore();
        const double getMaxBound();
        const double getMinBound();
        void setMinScore(double sc);
        void setMaxScore(double sc);
        void setMaxBound(double sc);
        void setMinBound(double sc);
        void printInfo(std::ofstream* );
};