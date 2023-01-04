#include "allIncludes.hpp"
#include "outputInfo.hpp"
#include "runCases.hpp"


void swap(outputInfo** xp, outputInfo** yp){
    outputInfo* temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
// bubble sort, to sort the vectors with the statistics with increasing order of file size
//must be called for each vector of each case we run
void bubbleSort(vector<outputInfo*>* arr, int n){
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr->at(j)->getSize() > arr->at(j+1)->getSize())
                swap(&arr->at(j), &arr->at(j+1));
}


void readFile(string input,vector<Point>* allPoints, double* chArea){
    //read contents of input file, ignoring first line with #
    fstream in;
    cout <<"name of file = " << input << endl;
    in.open(input, ios::in);
    if(!in){
        cout <<"error while opening file" << endl;
    }
    int totalPoints=0;//variable to store the total amount of 2D points given in input file
    int count=0;
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
            int wordCounter=0;
            while (token2) {
                wordCounter++;
                if(wordCounter==6){
                    *chArea=stod(token2);
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
        char delim2[3] = " \t";
        char tmp[text.size() + 1];
        strcpy(tmp, text.c_str());
        if(strlen(tmp) <= 2){
            continue;
        }
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
        allPoints->push_back(temp);
    }
    cout <<"total points = " << allPoints->size() << endl;

    in.close();
}

int readFolder(string path,ofstream* outfile, int preprocess){
    DIR *dir=opendir( path.c_str() );
    vector<outputInfo*> infoCase1;//for each case(combination of algorithms we have a separate vector for its ratio statistics)
    vector<outputInfo*> infoCase2;
    vector<outputInfo*> infoCase3;
    vector<outputInfo*> infoCase4;
    int totalSizes=0;//how many different totalPoint sizes we have
    double L;
    struct dirent *theDir;
    // try to open the path
    if ( dir  == NULL ){
        cout<<"Not a dir"<<endl;
        return -1;
    }

    // it is a folder
    string that("/");
    string newPath= path+that;

    int files=0;

    // read the folder
    while ( ( theDir=readdir(dir) ) != NULL ){ 
        // if the folder name is . or .. then do nothing
            if(strcmp(theDir->d_name,".")==0||strcmp(theDir->d_name,"..")==0)
                continue;
            string temp(theDir->d_name);
            // for all the files
            vector<Point> allPoints;
            double chArea;
            readFile(newPath+temp, &allPoints, &chArea);

            //update info case vectors
            int flag=0;
            for(int i=0; i<infoCase1.size(); i++){
                if(infoCase1.at(i)->getSize() == allPoints.size()){
                    flag=1;
                    break;
                }
            }
            //if flag=0 then a new file size was found so have to add a new element to all vectors of type <outputInfo*>
            if(!flag){
                outputInfo*  newSize1 = new outputInfo(allPoints.size());
                outputInfo*  newSize2 = new outputInfo(allPoints.size());
                outputInfo*  newSize3 = new outputInfo(allPoints.size());
                outputInfo*  newSize4 = new outputInfo(allPoints.size());
                infoCase1.push_back(newSize1);
                infoCase2.push_back(newSize2);
                infoCase3.push_back(newSize3);
                infoCase4.push_back(newSize4);
                totalSizes++;
            }

            //run case1
            runCase1(&allPoints, &infoCase1, chArea, preprocess);

            //run case2
            runCase2(&allPoints, &infoCase2, chArea, preprocess);

            //run case3
            runCase3(&allPoints, &infoCase3, chArea, preprocess);

            //run case4
            runCase4(&allPoints, &infoCase4, chArea, preprocess);
    
    }
    //sort all vectors in increasing order of size
    bubbleSort(&infoCase1, infoCase1.size());
    bubbleSort(&infoCase2, infoCase2.size());
    bubbleSort(&infoCase3, infoCase3.size());
    bubbleSort(&infoCase4, infoCase4.size());

    //after all cases are implemented for all files we print statistics
    for(int i=0; i<totalSizes; i++){
        *outfile << infoCase2.at(i)->getSize() << "\t\t";
        infoCase1.at(i)->printInfo(outfile);
        infoCase2.at(i)->printInfo(outfile);
        infoCase3.at(i)->printInfo(outfile);
        infoCase4.at(i)->printInfo(outfile);
        //then print info of next case
        *outfile << "||"<<endl;
    }

    //delete all vectors of type <outputInfo*>
    for (int i=0;i<infoCase1.size();i--){
        delete infoCase1.at(i);
        delete infoCase2.at(i);
        delete infoCase3.at(i);
        delete infoCase4.at(i);
    }
    return 0;
    
}
