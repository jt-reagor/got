#include<stdio.h>
#include<iostream>
#include<cstring>
using namespace std;

int init(char *name);

int main(int argc, char *argv[]){
    for(int i=1; i<argc; i++){
        printf("%d: %s\n", i, argv[i]);

        if(strcmp(argv[i], "init") == 0){
            if(argc - i <= 1){
                printf("Too few arguments\n");
                break;
            }
            int retCode = init(argv[++i]);
            printf("retCode of init = %d\n", retCode);
        }
    }
    return 0;
}

int init(char *name){
    int retcode;
    string nameString = string(name);

    printf("Initializing...\n");
    string cmd = "mkdir " + nameString;
    retcode = system(cmd.c_str());
    if(retcode != 0) return retcode;
    string cwd = "./" + nameString + "/";

    cmd = "mkdir " + cwd + "/.got";
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