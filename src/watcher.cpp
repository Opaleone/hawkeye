#include "../include/watcher.h"
#include "../include/utils.h"
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>

Watcher::Watcher(const Config &cfg) : config(cfg) {
  // Start the monitoring thread correctly using a lambda
  std::thread monitorThread(&Watcher::monitorProcesses, this);
  monitorThread.detach();
}

void Watcher::startListening() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == 0) {
    Utils::logMessage("Socket creation failed.");
    return;
  }

  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(config.port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    Utils::logMessage("Bind failed.");
    return;
  }

  if (listen(server_fd, 5) < 0) {
    Utils::logMessage("Listen failed.");
    return;
  }

  Utils::logMessage("Listening on port " + std::to_string(config.port));

  while (true) {
    new_socket =
        accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
      Utils::logMessage("Accept failed.");
      continue;
    }

    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    std::string receivedData(buffer);

    processIncomingData(receivedData);

    close(new_socket);
  }
}

void Watcher::processIncomingData(const std::string &data) {
  std::istringstream ss(data);
  int pid;
  std::string programName, secret;

  ss >> pid >> programName >> secret;

  if (secret != config.secret_key) {
    Utils::logMessage("Authentication failed.");
    return;
  }

  processes[pid] = std::make_pair(0, programName);

  Utils::logMessage("Monitoring PID: " + std::to_string(pid) + " (" +
                    programName + ")");
}

void Watcher::monitorProcesses() {
  while (true) { // Run forever
    std::this_thread::sleep_for(
        std::chrono::seconds(5)); // Check every 5 seconds

    for (auto it = processes.begin(); it != processes.end();) {
      int pid = it->first;
      std::string programName = it->second.second;

      if (!Utils::isProcessRunning(pid)) {
        Utils::logMessage("Process " + std::to_string(pid) + " (" +
                          programName + ") has stopped!");

        // Restart process if max restarts aren't exceeded
        int &restartCount = it->second.first;
        if (restartCount < config.max_restarts) {
          Utils::logMessage("Restarting " + programName + "... Attempt " +
                            std::to_string(restartCount + 1));
          restartCount++;
          Utils::restartProcess(config.script_paths[programName]);
        } else {
          Utils::logMessage("Max restart attempts reached for " + programName);
          Utils::sendEmailAlert(pid, "Max restarts reached for " + programName);
        }

        it = processes.erase(
            it); // Remove from monitoring if max retries exceeded
      } else {
        ++it;
      }
    }
  }
}
