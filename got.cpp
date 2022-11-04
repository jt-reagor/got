#include<stdio.h>
#include<iostream>
#include<cstring>
using namespace std;

int init(char *name);
int init();
int diff(string file1,string file2);

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
        else if (strcmp(argv[i],"diff")==0){
            if(argc - i <= 2){
                printf("Too few arguments\n");
                break;
            }
            else{
                diff(argv[2],argv[3]);
            }
        }

        //put rest of commands here in if statements like "init" above.
    }
    return 0;
}

// initializes .got folder in new directory
// TODO: clean up this code and add error handling using the retcodes.
int init(char *name){
    int retcode;
    string nameString = string(name);

    printf("Initializing...\n");
    string cmd = "mkdir " + nameString;
    retcode = system(cmd.c_str());
    if(retcode != 0) return retcode;
    string cwd = "./" + nameString + "/";

    cmd = "mkdir " + cwd + ".got";
    retcode = system(cmd.c_str());
    if(retcode != 0) return retcode;


    cwd.append(".got/");
    cmd = "mkdir " + cwd + "branches";
    retcode = system(cmd.c_str());
    cmd = "mkdir " + cwd + "hashes";
    retcode = system(cmd.c_str());
    cmd = "touch " + cwd + "log";
    retcode = system(cmd.c_str());

    printf("Initialized.\n");
    return 0;
}

// initializes .got folder in current directory
int init(){
    int retcode;

    printf("Initializing...\n");
    string cwd = "./";

    string cmd = "mkdir " + cwd + ".got";
    retcode = system(cmd.c_str());
    if(retcode != 0) return retcode;


    cwd.append(".got/");
    cmd = "mkdir " + cwd + "branches";
    retcode = system(cmd.c_str());
    cmd = "mkdir " + cwd + "hashes";
    retcode = system(cmd.c_str());
    cmd = "touch " + cwd + "log";
    retcode = system(cmd.c_str());

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
