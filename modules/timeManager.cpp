#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <signal.h>
#include "../include/timeManager.hpp"
int cutOff=0;
using namespace std;
void TimeHandler(int sig){
    if(sig==SIGXCPU){
        std::cout<<"CUT OFF\n";
        cutOff=1;
    }
}

int checkCutOf(){
    if(cutOff==1)
        return 1;
    return 0;
}


void initializeTime(int n){
    // check if there a previous Limit
    struct rlimit previousLimit;
    if(getrlimit(RLIMIT_CPU,&previousLimit)!=0){
        printf("setrlimit() failed with errno=%d\n", errno);
        exit(-1);
    }

    
    int previousCutOff=previousLimit.rlim_cur;
    int currentCutOff;
    // if there is not initialize the signal and make the current
    if(previousCutOff<=0){
        signal(SIGXCPU,TimeHandler);
        currentCutOff=n*5;
    }
    // else remember the time limit of the previous limit and increase with the new value
    else{
        currentCutOff=n*5+previousCutOff;
    }

    // add it as the new limit
    struct rlimit newLimit;
    newLimit.rlim_cur=currentCutOff;
    newLimit.rlim_max=RLIM_INFINITY;
    if (setrlimit(RLIMIT_CPU, &newLimit) != 0) {
        printf("setrlimit() failed with errno=%d\n", errno);
        exit(1);
    }

    // reset the cut off
    cutOff=0;
}