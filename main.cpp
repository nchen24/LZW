#include <iostream>
#include <fstream>
#include <map>
#include "LZW-a.h"
using namespace std;

int main(int argc, char* argv[]){
  if (argc != 3){
    cerr << "args fail\n";
    fprintf(stderr, "Usage: ./a.out [-c -x] [fileName]\n");
    exit(1);
  }

  if(strcmp(argv[1], "-x") && strcmp(argv[1], "-c")) {
    cerr << " x/c fail\n";
    fprintf(stderr, "Usage: ./a.out [-c -x] [fileName]\n");
    exit(1);
  }

  if(!fopen(argv[2], "r")){
    cerr << "Open fail\n";
    cerr << "Unable to open file\n";
    exit(1);
  }
  
  LZW myLZW(argv[2], argv[1][1]);
  myLZW.run();
}
