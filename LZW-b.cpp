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
  uint16_t nextCode = 257;
  unsigned it = 0;

  int c; 
  while((c = getc(fp)) != EOF){
    curString += c;
    if(!codes.count(curString)){
      codes[curString] = nextCode;
      nextCode++;
      if(nextCode >= 65535){
        cerr << "eojfirelkk\n";
      }

      curString.erase(curString.size() - 1);
      uint16_t temp = codes[curString];
      unsigned char left  = temp >> 8;
      unsigned char right = temp & 0x00ff;

      putc(left,  stdout); // Leftmost  8 bits
      putc(right, stdout); // Rightmost 8 bits

      curString = c;
    }
    it++;
  }

  putc(0x01, stdout); putc(0x00, stdout); // EOF marker
}

void LZW::extractLZW(){
  string prevString = "";
  string curString  = "";
  unsigned int nextCode = 257;

  uint16_t code;

  string s;
  ifstream fin("out");
  do{
    int temp = getc(fp);
    if(feof(fp))
      break;
    code = temp << 8;
    temp = getc(fp);
    assert(!feof(fp));
    code += temp;
    
    if (code == 256){
      cerr << "Normal break\n";
      break;
    }
    if(!strings.count(code))
      strings[code] = prevString + prevString[0];

    curString = strings[code];
    cout << curString;
    if (prevString.length()){
      strings[nextCode] = prevString + curString[0];
      nextCode++;
    }
    prevString = curString;

  }while(1);

}
