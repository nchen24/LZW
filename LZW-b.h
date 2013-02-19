#include <map>
#include <string>
//using namespace std;

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
    std::map<unsigned int, std::string> strings;

};
