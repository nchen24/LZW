#include <iostream>
#include <fstream>
#include <assert.h>
#include "LZW-a.h"
using namespace std;

LZW::LZW(char *fileName, char runMode){
  mode = runMode;
  fp = fopen(fileName, "r");

  string s;
  for (unsigned i = 0 ; i < 256 ; i++){
    s = (char)i;
    codes[s] = i;
    strings[i] = s;
  }
}

LZW::~LZW(){
  fclose(fp);
}

void LZW::run(){
  if(mode == 'c')
    compressLZW();
  else
    extractLZW();
}

void LZW::compressLZW(){
  string curString = "";
  unsigned nextCode = 257;
  unsigned it = 0;

  int c; 
  while((c = getc(fp)) != EOF){
    curString += c;
    if(!codes.count(curString)){
      codes[curString] = nextCode;
      nextCode++;
      curString.erase(curString.size() - 1);
      cout << codes[curString] << " ";
      curString = c;
    }
    it++;
  }
  if(codes.count(curString))
    cout << codes[curString] << " ";
  cout << "256"; // EOF marker
}

void LZW::extractLZW(){
  string prevString = "";
  string curString  = "";
  unsigned int nextCode = 257;

  int code;
  string strCode = "";

  ifstream fin("out");
  while(fin >> strCode){
    //Convert to int
    code = atoi(strCode.c_str());
    if (code == 256)
      break;
    if(!strings.count(code))
      strings[code] = prevString + prevString[0];

    curString = strings[code];
    cout << curString;
    if (prevString.length()){
      strings[nextCode] = prevString + curString[0];
      nextCode++;
    }
    prevString = curString;
    strCode = "";
  }
}
