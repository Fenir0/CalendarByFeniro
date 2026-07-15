#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ResourceManager{
    public:
    static json loadAllFromFile(const std::string& filename);
    static void saveAllIntoFile(const std::string& filename, json data);
    static void removeFile(const std::string& filename);

};

#endif