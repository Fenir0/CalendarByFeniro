#include "../inc/resourceManager.h"

json ResourceManager::loadAllFromFile(const std::string& filename)
{
    try{
    std::fstream f(filename);
    json data = json::parse(f);
    }
    catch(std::exception e){
        std::cerr << "Error while loading file\n";
    }
}

void ResourceManager::saveAllIntoFile(const std::string& filename, json data)
{
    try{
    std::fstream f(filename);
    f << data << '\n';
    }
    catch(std::exception e){
        std::cerr << "Error while writing into file\n";
    }
}

void ResourceManager::removeFile(const std::string& filename)
{
    int status = remove(filename.c_str());
}
