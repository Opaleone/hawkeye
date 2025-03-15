#include "../include/utils.h"
#include "../include/config.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdlib>
#include <signal.h>

void Utils::logMessage(const std::string &message) {
  std::ofstream logFile("hawkeye.log", std::ios::app);
  if (logFile.is_open()) {
    logFile << message << "\n";
    logFile.close();
  } else {
    std::cerr << "Failed to open log file." << std::endl;
  }
};

bool Utils::checkProcessErrors(pid_t pid, const std::vector<std::string> &errorKeywords) {
  std::ifstream errorFile("/proc/" + std::to_string(pid) + "/fd/2");
  if (!errorFile.is_open()) return false;

  std::string line;
  while (std::getline(errorFile, line)) {
    for (std::vector<std::string>::const_iterator it = errorKeywords.begin(); it != errorKeywords.end(); ++it) {
      if (line.find(*it) != std::string::npos) {
        return true;
      }
    }
  }
  return false;
};

void Utils::sendEmailAlert(int pid, const std::string &message) {
  Config cfg;

  std::string command = "echo 'Process " + std::to_string(pid) + " encountered an error: " + message + "' | mail -s 'Process Failure Alert' " + cfg.email;
  if (system(command.c_str()) != 0) {
    logMessage("Failed to send email alert for PID: " + std::to_string(pid));
  } else {
    logMessage("Sent email alert for PID: " + std::to_string(pid));
  }
};

bool Utils::isProcessRunning(pid_t pid) {
  return (kill(pid, 0) == 0);
}