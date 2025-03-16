#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>
#include <vector>

class Config
{
public:
    std::string secret_key;
    std::unordered_map<std::string, std::string> script_paths;
    std::vector<std::string> error_keywords;
    std::string log_file;
    std::string email;
    int max_restarts;
    int port;

    bool loadFromFile(const std::string &filename); // Loads from .cfg file
};

#endif