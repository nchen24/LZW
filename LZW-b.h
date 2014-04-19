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

    void initMap();
    void resetMap();
    std::map<std::string, uint16_t> codes;
    std::map<uint16_t, std::string> strings;
};
