#include "resources.hpp"
#include <iostream>
#include <fstream>
#include <string>

std::string Resources::ImgPath() { 
  return "/img/map";
};

bool Resources::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
};