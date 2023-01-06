#include "allIncludes.hpp"
#include "mainUtil.hpp"


int main(int argc, char* argv[]){ 

    if (argc!=7){
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }

    string folderInput;
    if (strcmp(argv[1],"-i")==0){
        string temp(argv[2]);
        folderInput=temp;
    }
    else if(strcmp(argv[3],"-i")==0){
        string temp(argv[4]);
        folderInput=temp;
    }
    else if(strcmp(argv[5],"-i")==0){
        string temp(argv[6]);
        folderInput=temp;
    }
    else{
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }

    string fileOutput;
    if (strcmp(argv[1],"-o")==0){
        string temp(argv[2]);
        fileOutput=temp;
    }
    else if(strcmp(argv[3],"-o")==0){
        string temp(argv[4]);
        fileOutput=temp;
    }
    else if(strcmp(argv[5],"-o")==0){
        string temp(argv[6]);
        fileOutput=temp;
    }
    else{
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }

    int preprocess;
    if (strcmp(argv[1],"-preprocess")==0){
        preprocess=atoi(argv[2]);
    }
    else if(strcmp(argv[3],"-preprocess")==0){
        preprocess=atoi(argv[4]);
    }
    else if(strcmp(argv[5],"-preprocess")==0){
        preprocess=atoi(argv[6]);
    }
    else{
        cout<<"Incorrect arguments"<<endl;
        return -1;
    }
    cout<< folderInput<<endl;
    cout << fileOutput<<endl;
    cout<< preprocess<<endl;

    unsigned int tmp = (unsigned) time(NULL);
    srand(tmp);
    ofstream outfile(fileOutput);
    outfile <<"\t\t\t";
    for (int i=0;i<CASES;i++)
        outfile <<"||\t\t\t\t\t\t\tAlgorithm\t"<<i+1<<"\t\t\t\t\t\t\t";
    outfile<<"||"<<endl;
    outfile <<"Size \t\t";
    for (int i=0;i<CASES;i++)
        outfile <<"||\t"  <<"Min Score \t\t"<<"Max Score\t\t" <<"Min Bound\t\t"<< "Max Bound\t\t";
    outfile<<"||"<<endl;

    readFolder(folderInput, &outfile, preprocess);

    outfile.close();


    return 0;
}
