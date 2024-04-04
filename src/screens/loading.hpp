#include <emscripten/fetch.h>
// #include "../util/Request.hpp"
class Loading {
    public:
        Loading() {}
        ~Loading() {}
        void fetchInitialFiles();
    private:
        emscripten_fetch_attr_t attr;
};