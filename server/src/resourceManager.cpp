#include "../inc/resourceManager.h"

static std::filesystem::path getBasePath(){
    const char* env = std::getenv("CALENDAR_SAVE_DIR");
    if(env) return std::filesystem::path(env);

    #ifdef _WIN32
        const char* home = std::getenv("USERPROFILE");
    #else
        const char* home = std::getenv("HOME");
    #endif
    
    if (home) return std::filesystem::path(home) / "CalendarByFeniro" / "saved";
    return std::filesystem::path("saved"); 
}

void ResourceManager::createUserFolder(uint32_t user_id)
{
    std::filesystem::path base_path = getBasePath().string() + "/";

    std::filesystem::path user_dir = base_path / std::to_string(user_id);
    std::filesystem::create_directories(user_dir);
}

json ResourceManager::loadAllFromFile(const std::string& userfolder, const std::string& file) 
{
    try{
        std::ifstream f(getBasePath().string() + "/" +userfolder + "/" + file + ".json");
        json data = json::parse(f);
        f.close();
        return data;
    }
    catch(std::exception e){
        std::cerr << "Error while loading file\n";
        json data = {};
        return data;
    }
}

void ResourceManager::saveAllIntoFile(const std::string& userfolder, const std::string& file, json data)
{
    try{
        std::ofstream f(getBasePath().string() + "/" +userfolder + "/" + file + ".json");
        f << data;
        std::string s = data.dump();
        f.close();
    }
    catch(std::exception e){
        std::cerr << "Error while writing into file\n";
    }
}

void ResourceManager::removeFile(const std::string& userfolder, const std::string& file)
{
    try{
        std::filesystem::remove(getBasePath().string() + "/" +userfolder + "/" + file + ".json");
    }
    catch(std::exception e){
        std::cerr << "Error while removing file\n";
    }
}
