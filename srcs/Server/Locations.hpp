#pragma once

#include <common_inc.h>

class Locations {
public:
    Locations() {}

    bool hasLocation(const std::string& locationName);
    // return path 
    std::string getRoot(const std::string& locationName);
    void addLocation(const std::string& locationName, const std::string& rootPath);
    bool checkIsEmpty();

    Locations(const Locations& /*orig*/) = delete;
    Locations& operator=(const Locations& /*orig*/) = delete;
#ifdef _DEBUG
    void printLocations();
#endif

private:
    // key is location, value is its path
    std::map<std::string, std::string> locations;
};
