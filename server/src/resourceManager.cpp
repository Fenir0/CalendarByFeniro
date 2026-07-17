#include "../inc/resourceManager.h"

void ResourceManager::createUserFolder(uint32_t user_id)
{
    // temporary
    std::filesystem::path base_path = "/home/aleksfeniro/2026_SUMMER_PRACTICE/saved";

    std::filesystem::path user_dir = base_path / std::to_string(user_id);
    std::filesystem::create_directories(user_dir);
}

json ResourceManager::loadAllFromFile(const std::string& userfolder, const std::string& file) 
{
    try{
        std::ifstream f("/home/aleksfeniro/2026_SUMMER_PRACTICE/saved/" +userfolder + "/" + file + ".json");
        json data = json::parse(f);
        f.close();
        return data;
    }
    catch(std::exception e){
        std::cerr << "Error while loading file\n";
    }
}

void ResourceManager::saveAllIntoFile(const std::string& userfolder, const std::string& file, json data)
{
    try{
        std::ofstream f("/home/aleksfeniro/2026_SUMMER_PRACTICE/saved/" +userfolder + "/" + file + ".json", std::ios::trunc);
        f << data;
        f.close();
    }
    catch(std::exception e){
        std::cerr << "Error while writing into file\n";
    }
}

void ResourceManager::updateFile(const std::string& userfolder, const std::string& file, json data)
{
}
void ResourceManager::removeFile(const std::string& userfolder, const std::string& file)
{
   //int status = remove(filename.c_str());
}
