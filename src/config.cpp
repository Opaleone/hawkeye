#include "../include/config.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool Config::loadFromFile(const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Could not open config file.\n";
    return false;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue; // Skip empty lines and comments
    std::istringstream ss(line);
    std::string key, value;
    if (std::getline(ss, key, '=') && std::getline(ss, value)) {
      if (key == "secret_key")
        secret_key = value;
      else if (key == "log_file")
        log_file = value;
      else if (key == "port")
        port = std::stoi(value);
      else if (key == "max_restarts")
        max_restarts = std::stoi(value);
      else if (key.find("script_") == 0)
        script_paths[key.substr(7)] = value;
      else if (key.find("error_") == 0)
        error_keywords.push_back(value);
      else if (key.find("email") == 0)
        email = value;
    }
  }

  return true;
}
