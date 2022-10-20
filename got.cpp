#include<stdio.h>
#include<iostream>
using namespace std;

int main(){
    while (true){
        char command[50];
        cout << ">";
        cin.getline(command, 50);
        cout << endl << command << endl;
    }
    return 0;
}