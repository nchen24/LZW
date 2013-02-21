#include <map>
#include <string>
#include <stdint.h>

class LZW{
  public:
    LZW(char *fileName, char runMode);
    ~LZW();

    void run();
  private:
    FILE *fp;
    char mode;

    void compressLZW();
    void extractLZW();

    std::map<std::string, unsigned int> codes;
    std::map<uint16_t, std::string> strings;

};
