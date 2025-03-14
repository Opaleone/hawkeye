#include "include/config.h"
#include "include/watcher.h"
#include <iostream>


int main() {
  Config config;
  if (!config.load("config.json")) {
    std::cerr << "Failed to load config file. Exiting...\n";
    return 1;
  }
  return 0;
}