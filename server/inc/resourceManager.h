#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <filesystem>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ResourceManager{
    public:
    static void createUserFolder(uint32_t user_id);
    static json loadAllFromFile(const std::string& userfolder, const std::string& file);
    static void saveAllIntoFile(const std::string& userfolder, const std::string& file, json data);
    static void updateFile     (const std::string& userfolder, const std::string& file, json data);
    static void removeFile     (const std::string& userfolder, const std::string& file);

};

#endif