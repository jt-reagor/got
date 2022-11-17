#include<stdio.h>
#include<iostream>
#include<fstream>
#include<cstring>
#include<sys/stat.h>
#include<chrono>
#include<ctime>
using namespace std;

int init(char *name);
int init();
int init_aux(string cwd);
int diff(string file1,string file2, string newfname);
int patch(string fileToPatch, string PatchFile);
string hasher(string name, string time);
int commit();
int log(string hash1,string hash2, string time, string user);

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
                diff(argv[2],argv[3], "test");
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
        else if (strcmp(argv[i], "commit")==0){
            int retcode = commit();
            printf("retcode of commit = %d\n", retcode);
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
    retcode = mkdir((cwd+"copy").c_str(), 0777);
    if(retcode != 0) return retcode;
    
    // make a blank TARGET file in copy
    ofstream targetFile((cwd+"/copy/TARGET").c_str());
    targetFile.close();

    // make log file in .got
    ofstream logFile((cwd+"log").c_str());
    logFile << "Log" << "\n--------------------------------------\n";
    time_t now = time(0);
    logFile << "Initialized " << ctime(&now);
    logFile.close();

    // make guser file
    ofstream guserFile((cwd+"guser").c_str());
    cout << "Enter your email: ";
    string email = "";
    cin >> email;
    cout << "Enter your username: ";
    string user = "";
    cin >> user;
    guserFile << email << endl << user;
    guserFile.close();

    // make master branch file
    ofstream mbranchFile((cwd+"branches/master").c_str());
    mbranchFile << "0";
    mbranchFile.close();

    printf("Initialized.\n");
    return 0;
}

// steps of a commit:
// 1. create unique id (hash) based on time and person committing TODO: make hash more robust
// 2. diff between current workspace and copy of last commit stored in .got folder
// 3. update log with previous hash, new hash, time, and user
// 4. update branch head to reflect current commit hash
// 5. copy contents of workspace into .got folder
int commit(){
    // step 1
    time_t now = time(0);
    // dt Formatted as Tue Nov 15 14:30:10 2022
    string dt = ctime(&now);
    dt.pop_back();
    // reads in the email and user from guser
    ifstream guser;
    guser.open(".got/guser");
    string email;
    getline(guser, email);
    string user;
    getline(guser, user);
    string commit_hash = hasher(user,dt);
    // step 2
    int retcode = diff("TARGET", ".got/copy/TARGET", commit_hash);
    // step 3
    // get previous commit hash from .got/branches/master file
    // user commit_hash for new hash
    ifstream myFile;
    myFile.open(".got/branches/master");
    string prev_hash;
    getline(myFile, prev_hash);
    log(prev_hash, commit_hash, dt, user);

    // step 4
    // change .got/branches/master file to contain hash of new commit

    // step 5

    return 0;
}

//gets the diff of two files and puts it to patch file
//TODO: add error handling
//TODO: add option to specify patch file
//TODO: outfile needs to be named the hash of the commit
int diff(string file1,string file2, string newfname){
    string cmd = "diff --color=auto " + file1 + " " + file2 + " >> .got/hashes/" + newfname;
    int retcode = system(cmd.c_str());
    return retcode;
}



//applies an inputted patch file to a file
//this will update the first input with the patch file in the second input
//TODO: add scalability to work with directories 
int patch(string fileToPatch, string PatchFile){
    string cmd = "patch -r " + fileToPatch + " -i " + PatchFile;
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
    string commitLog = hash1 + " -> " + hash2 + " " + time + " " + user+ "\n";
    ofstream myFile;
    myFile.open(".got/log", ios::app);
    myFile << commitLog;
    myFile.close();
    return 0;
}

