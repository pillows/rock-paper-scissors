#include <string>

class Resources {
    public:
    Resources();
    ~Resources();
    std::string ImgPath();
bool fileExists(const std::string& filename);
};
