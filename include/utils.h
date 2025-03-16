#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils
{
  void logMessage(const std::string &message);
  bool checkProcessErrors(pid_t pid, const std::vector<std::string> &errorKeywords);
  void restartProcess(const std::string &scriptPath);
  void sendEmailAlert(int pid, const std::string &message);
  bool isProcessRunning(pid_t pid);
};

#endif