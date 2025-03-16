#ifndef WATCHER_H
#define WATCHER_H

#include "../include/config.h"
#include <unordered_map>
#include <string>

class Watcher
{
public:
  explicit Watcher(const Config &cfg);
  void startListening();
  void monitorProcesses();
  void processIncomingData(const std::string &data);

private:
  Config config;
  // PID -> (restart count, program name)
  std::unordered_map<int, std::pair<int, std::string>> processes;
};

#endif