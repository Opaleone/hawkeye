#include "../include/watcher.h"
#include "../include/utils.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>

Watcher::Watcher(const Config &cfg) : config(cfg) {}

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

  if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    Utils::logMessage("Bind failed.");
    return;
  }

  if (listen(server_fd, 5) < 0) {
    Utils::logMessage("Listen failed.");
    return;
  }

  Utils::logMessage("Listening on port " + std::to_string(config.port));
}