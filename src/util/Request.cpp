#include "Request.hpp"
#include <cassert> 
#include <iostream>
#include <fstream>
#include <functional> // for std::bind
#include <emscripten/fetch.h>
#include "imgparse/accessor.hpp"
#include "imgparse/imgfile.hpp"
#include <nlohmann/json.hpp>
#include "constants.hpp"
// #include <string>

std::string RequestIMG::fileName;
using json = nlohmann::json;

std::string ImgPath() { 
  return "/img";
}

bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}


// void RequestIMG::downloadFailed(emscripten_fetch_t *fetch) {
//     assert(fetch);
// }

void Request::downloadSucceeded(emscripten_fetch_t *fetch) {
    assert(fetch);
}

void Request::downloadFailed(emscripten_fetch_t *fetch) {
    assert(fetch);
}

void RequestIMG::setFileName(std::string name) {
    fileName = name;
}

std::string RequestIMG::getFileName() {
    return fileName;
}

std::string RequestIMG::staticGetFileName(const std::string& fileName) {
    return fileName;
}

void parseIMGFile() {
    // auto accessor = new Accessor();
    // accessor->fetchImgFile("UI/Basic");
    // json payload = accessor->getPayload();
    // std::cout << payload << std::endl;
    std::map<std::string, std::unique_ptr<IMGFile>> files;
    std::string imgPath = "/img/UI/Basic.img";
    auto absPath = std::filesystem::absolute(imgPath);
    std::cout <<  "created abs path: " << absPath << std::endl;
    // std::cout << "current path: " << absPath << std::endl;
    files.emplace(imgPath, std::unique_ptr<IMGFile>(
      new IMGFile(imgPath)));
    json obj = json::object();
    // auto file = new IMGFile(absPath);

    // std::string depth = "NameChange.Text00";
    // auto data = files["NameChange.img"]->extract(depth);
    // auto data = files[a;
    // for (auto it = files.begin(); it != files.end(); ++it) {
    //   std::cout << "hi" << std::endl;
    //     it->second->print();
    // }
    std::cout << "extracting" << std::endl;

    // files[absPath]->extract();
    // files[absPath]->initKey();
    // std::cout << files[absPath]->getJson() << std::endl;
    auto imgJson = files[absPath]->getJson();
    auto cursorJson = imgJson[absPath]["Cursor"]["0"];
    std::cout << cursorJson.dump(2) << std::endl;
    // auto data = imgJson[absPath];
    // // std::cout << imgJson.dump(2) << std::endl;
    
    // std::vector<std::string> keys;
    // for (json::iterator it = data.begin(); it != data.end(); ++it) {
    //     keys.push_back(it.key());
    // }

    // // Print keys
    // for (const std::string& key : keys) {
    //     std::cout << key << std::endl;
    // }

}

void RequestIMG::fetchFile(std::string url, std::string filePath) {
    setFileName(filePath);
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY | EMSCRIPTEN_FETCH_PERSIST_FILE;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    attr.userData = this;
    std::cout << "file path: " << filePath << std::endl;
    emscripten_fetch(&attr, url.c_str());
}

void RequestIMG::downloadSucceeded(emscripten_fetch_t *fetch) {
  // printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
  // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
std::string filePath = ImgPath() + "/" + fileName;
std::cout << "file path to save: " << filePath << std::endl;
  if (!fileExists(filePath)) {
    std::ofstream myFile;
    myFile.open(filePath, std::ios::out | std::ios::binary);
    myFile.write (fetch->data, fetch->numBytes);
    myFile.close();
    std::cout << "Since file doesn't exist we create it" << std::endl;
  }


  parseIMGFile();

  std::cout << "downloadSucceeded" << std::endl;
  emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void RequestIMG::downloadFailed(emscripten_fetch_t *fetch) {
  std::cout << "downloadFailed" << std::endl;
  // printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
  emscripten_fetch_close(fetch); // Also free data on failure.
}