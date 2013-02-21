#include <iostream>
#include <assert.h>
#include "LZW-a.h"
using namespace std;

//Emits 'code' to fp in the form of two bytes. 
static inline void shortToOutStream(FILE* fp, uint16_t code){
  unsigned char left  = code >> 8;     //Shift right to get left 8 bits
  unsigned char right = code & 0x00ff; //Mask off the left 8 bits
  putc(left,  fp); 
  putc(right, fp); 
}

//Gets two bytes from fp, stores them in the '*code'.  Returns true on success.
static inline bool inStreamToShort(FILE *fp, uint16_t *code){
  int c = getc(fp);
  if(feof(fp))
    return false;
  *code = c << 8;
  c = getc(fp);
  if(feof(fp))
    return false;
  *code += c;
  return true;
}

LZW::LZW(char *fileName, char runMode){
  mode = runMode;
  if(!strcmp(fileName, "-"))
    fp = stdin;
  else
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
  int c; 

  while((c = getc(fp)) != EOF){
    curString += c;
    if(!codes.count(curString)){
      codes[curString] = nextCode;
      nextCode++;
      assert(nextCode != 65535);

      curString.erase(curString.size() - 1);
      shortToOutStream(stdout, codes[curString]);

      curString = c;
    }
  }
  if(codes.count(curString))
      shortToOutStream(stdout, codes[curString]);
  shortToOutStream(stdout, 256); // EOF marker
}

void LZW::extractLZW(){
  string prevString = ""; string curString  = "";
  uint16_t nextCode = 257;
  uint16_t code;

  while(inStreamToShort(fp, &code)){
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
  }
}
