#include <iostream>
#include <assert.h>
#include "LZW-b.h"
using namespace std;

static const uint16_t FIRST_CODE = 258;
static const uint16_t RESET_CODE = 65535; 

//Emits 'code' to fp in the form of two bytes. 
static inline void shortToOutStream(FILE* fp, uint16_t code){
  unsigned char left  = code >> 8;     //Shift off the right 8 bits
  unsigned char right = code & 0x00ff; //Mask  off the left 8 bits
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

//Depending on rt args, sets fp to stdin or file, then initializes the maps.
LZW::LZW(char *fileName, char runMode){
  mode = runMode;
  if(!strcmp(fileName, "-"))
    fp = stdin;
  else
    fp = fopen(fileName, "r");
  initMap();
}
LZW::~LZW(){
  fclose(fp);
}

//Inits the maps.  Separated from LZW::LZW b/c the maps reset if 16b is insuf.
void LZW::initMap(){
  string s;
  for (unsigned i = 0 ; i < 256 ; i++){
    s = (char)i;
    codes[s] = i;
    strings[i] = s;
  }
}

//Wrapper for clearing maps, re-init'ing them
void LZW::resetMap(){
  codes.clear();
  strings.clear();
  initMap();
}


void LZW::run(){
  if(mode == 'c')
    compressLZW();
  else
    extractLZW();
}

//Reads from stream until it forms a string that has not yet been added to the
//map.  It adds that string to the map, then emits the code for curString minus
//the last letter.  The loop then continues, starting with the last letter.  If
//the entire map is filled (because this implementation uses shorts, that means
//65535 strings), it resets the map and continues reading.
void LZW::compressLZW(){
  string curString = "";
  uint16_t nextCode = FIRST_CODE;
  int c; 

  while((c = getc(fp)) != EOF){
    curString += c;
    if(!codes.count(curString)){
      codes[curString] = nextCode;
      curString.erase(curString.size() - 1);
      shortToOutStream(stdout, codes[curString]);

      nextCode++;

      if(nextCode == RESET_CODE){
        nextCode = FIRST_CODE;
        resetMap();
        shortToOutStream(stdout, 257);
      }
      curString = c;
    }
  }
  if(codes.count(curString)) //For edge cases, w/o sometimes truncs. last char
    shortToOutStream(stdout, codes[curString]);
  shortToOutStream(stdout, 256); //EOF marker
}

//Reads two bytes from the stream at a time, decoding them into a single short.
//It then builds the map as it goes, resetting appropriately if the map becomes
//too large.
void LZW::extractLZW(){
  string prevString = ""; string curString  = "";
  uint16_t nextCode = FIRST_CODE;
  uint16_t code;

  while(inStreamToShort(fp, &code)){
    if (code == 256)
      break;
    else if (code == 257){
      nextCode = FIRST_CODE;
      prevString = ""; curString = "";
      resetMap();
    }
    else{
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
}
