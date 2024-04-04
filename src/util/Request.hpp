#pragma once

#include <emscripten/fetch.h>
#include <string>
#include <iostream>
#include <cassert>


// class URLData {
//   public:
//     URLData(std::string url, std::string filePath) : url(url), filePath(filePath) {}
//     std::string url;
//     std::string filePath;
// };

// class RequestIMG {
//   public:
//     RequestIMG() {}
//     ~RequestIMG() {}
//     void fetchFile(std::string url, std::string filePath);
//     void downloadSucceeded(emscripten_fetch_t* fetch);
//     void downloadFailed(emscripten_fetch_t* fetch);

//   private:
//     emscripten_fetch_attr_t attr;
// };


class Request {
public:
    Request() {
        // Assign member function callbacks
        attr.onsuccess = onSuccessCallback;
        attr.onerror = onErrorCallback;
        attr.onprogress = onProgressCallback;

        // Assign 'this' pointer to user data for callbacks
        attr.userData = this;
    }

    // Static member functions to use as callbacks
    static void onSuccessCallback(emscripten_fetch_t* fetch) {
        Request* request = static_cast<Request*>(fetch->userData);
        request->downloadSucceeded(fetch);
    }

    static void onErrorCallback(emscripten_fetch_t* fetch) {
        Request* request = static_cast<Request*>(fetch->userData);
        request->downloadFailed(fetch);
    }

    static void onProgressCallback(emscripten_fetch_t* fetch) {
      assert(fetch);
        // Implementation for onProgressCallback function
    }

    virtual ~Request() {}  // Virtual destructor
    virtual void execute() = 0;

    void fetchFile(std::string url, std::string filePath);
    


private:
    emscripten_fetch_attr_t attr;
    static void downloadSucceeded(emscripten_fetch_t* fetch);

    static void downloadFailed(emscripten_fetch_t* fetch);
};

class RequestIMG : public Request {
public:
    void execute() override {
        // Provide implementation for the execute function
        std::cout << "Executing request for image." << std::endl;
    }
    void fetchFile(std::string url, std::string filePath);
    static void downloadSucceeded(emscripten_fetch_t* fetch);

    static void downloadFailed(emscripten_fetch_t* fetch);
    std::string getFileName(); // Getter function for fileName
    void setFileName(std::string name);  // Add this line
    std::string staticGetFileName(const std::string& fileName);
    static std::string fileName;
    
  private:
  static std::string staticGetFileName(); // Static getter function for fileName
    
    emscripten_fetch_attr_t attr;
};