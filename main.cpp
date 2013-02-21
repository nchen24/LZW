#include <iostream>
#include <fstream>
#include <map>
#include "LZW-a.h"
using namespace std;

int main(int argc, char* argv[]){
  if (argc != 3){
    fprintf(stderr, "Improper number of arguments.\n");
    fprintf(stderr, "Usage: ./a.out [-c -x] [fileName]\n");
    exit(1);
  }

  if(strcmp(argv[1], "-x") && strcmp(argv[1], "-c")) {
    fprintf(stderr, "Improper run option.\n");
    fprintf(stderr, "Usage: ./a.out [-c -x] [fileName]\n");
    exit(1);
  }

  if(strcmp(argv[2], "-") && !fopen(argv[2], "r")){
    fprintf(stderr, "Unable to open file.\n");
    fprintf(stderr, "Usage: ./a.out [-c -x] [fileName]\n");
    exit(1);
  }
  
  LZW myLZW(argv[2], argv[1][1]);
  myLZW.run();
}
