#include <iostream>
#include <fstream>
#include <assert.h>
#include "LZW-a.h"
using namespace std;

struct OneCode{
  unsigned char left;
  unsigned char right;
};

static inline OneCode shortToBytes(uint16_t code){
  OneCode codeInBytes;
  codeInBytes.left  = code >> 8;     //Shift right to get left 8 bits
  codeInBytes.right = code & 0x00ff; //Mask off the left 8 bits
  return codeInBytes;
}

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
      assert(nextCode != 65535);

      curString.erase(curString.size() - 1);
      OneCode temp = shortToBytes(codes[curString]);
      putc(temp.left,  stdout); // Leftmost  8 bits
      putc(temp.right, stdout); // Rightmost 8 bits

      curString = c;
    }
    it++;
  }

  if(codes.count(curString)){
      OneCode temp = shortToBytes(codes[curString]);
      putc(temp.left,  stdout); // Leftmost  8 bits
      putc(temp.right, stdout); // Rightmost 8 bits
  }

  putc(0x01, stdout); putc(0x00, stdout); // EOF marker
}

void LZW::extractLZW(){
  string prevString = "";
  string curString  = "";
  unsigned int nextCode = 257;

  uint16_t code;

  string s;
  int c;
  ifstream fin("out");
  while((c= getc(fp)) != EOF){
    code = c << 8;
    c = getc(fp);
    assert(!feof(fp));
    code += c;
    
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

  }

}
