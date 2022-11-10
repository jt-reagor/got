#include<stdio.h>
#include<iostream>
#include<fstream>
#include<cstring>
#include<sys/stat.h>
#include<chrono>
using namespace std;

int init(char *name);
int init();
int init_aux(string cwd);
int diff(string file1,string file2);


int main(int argc, char *argv[]){
    for(int i=1; i<argc; i++){
        // printf("%d: %s\n", i, argv[i]);
int main(int argc, char *argv[]){
    for(int i=1; i<argc; i++){
        // printf("%d: %s\n", i, argv[i]);
        //init
        if(strcmp(argv[i], "init") == 0){
            if(argc - i > 1){
                int retcode = init(argv[++i]);
                printf("retcode of init = %d\n", retcode);
            }else{
                int retcode = init();
                printf("retcode of init = %d\n", retcode);
            }
        }
        //diff
        else if (strcmp(argv[i],"diff")==0){
            if(argc - i <= 2){
                printf("Too few arguments\n");
                break;
            }
            else{
                diff(argv[2],argv[3]);
            }
        }
        //patch
        else if (strcmp(argv[i],"patch")==0){
            if(argc - i <= 2){
                printf("Too few arguments\n");
                break;
            }
            else{
                patch(argv[2],argv[3]);
            }
        }
        //put rest of commands here in if statements like "init" above.
    }
    return 0;
}

// initializes .got folder in new directory
// TODO: clean up this code and add error handling using the retcodes.
int init(char *name){
    printf("Initializing...\n");
    // make new directory of name specified by arg
    int retcode = mkdir(name, 0777);
    if(retcode != 0) return retcode;
    string nameString = string(name);
    string cwd = "./" + nameString + "/";

    // pass to aux func that constructs .got dir
    return init_aux(cwd);
}

// initializes .got folder in current directory
int init(){
    printf("Initializing...\n");
    string cwd = "./";

    //pass to aux func that constructs .got dir
    return init_aux(cwd);
}

int init_aux(string cwd){
    // make .got dir
    int retcode = mkdir((cwd+".got").c_str(), 0777);
    if(retcode != 0) return retcode;

    // make branches and hashes dirs in .got
    cwd.append(".got/");
    retcode = mkdir((cwd+"branches").c_str(), 0777);
    if(retcode != 0) return retcode;
    retcode = mkdir((cwd+"hashes").c_str(), 0777);
    if(retcode != 0) return retcode;
    
    // make log file in .got
    ofstream logFile((cwd+"log").c_str());
    logFile << "Log" << "\n--------------------------------------\n";
    // auto time = chrono::system_clock::now();
    // logFile << "Initialized " << ctime(&time);
    logFile.close();

    printf("Initialized.\n");
    return 0;
}

//gets the diff of two files and puts it to patch file
//TODO: add error handling
//TODO: add option to specify patch file
//TODO: outfile needs to be named the hash of the commit
int diff(string file1,string file2){
    string cmd = "diff --color=auto " + file1 + " " + file2 + " >> testing.patch";
    int retcode = system(cmd.c_str());
    return retcode;
}



//applies an inputted patch file to a file
//this will update the first input with the patch file in the second input
//TODO: add scalability to work with directories 
int patch(string fileToPatch, string PatchFile){
    string cmd = "patch " + fileToPatch + " -i " + PatchFile;
    int retcode = system(cmd.c_str());
    return retcode; 
}
//hashes the 2 strings and returns the hash
//TODO: add error handling
string hasher(string name, string time){
    string hash = name + time;
    unsigned long hashVal = 5381;
    int c;
    for(int i=0; i<hash.length(); i++){
        c = hash[i];
        hashVal = ((hashVal << 5) + hashVal) + c;
    }
    return to_string(hashVal);
}
int log(string hash1,string hash2, string time, string user){
    string commitLog = hash1 + " -> " + hash2 + "     " + time + "     " + user;
    ofstream myFile;
    myFile.open("/got/test/.got/log");
    myFile << commitLog;
}

