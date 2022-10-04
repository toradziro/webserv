#pragma once

#include <common_inc.h>

class Locations {
public:
    explicit Locations() {}
    bool hasLocation(const std::string& locationName) {
        if(locations.count(locationName) == 0) {
            return false;
        }
        return true;
    }

    // return path 
    std::string getRoot(const std::string& locationName) {
        return locations[locationName];
    }

    void addLocation(const std::string& locationName, const std::string& rootPath) {
        locations[locationName] = rootPath;
    }

#ifdef _DEBUG
    void printLocations() {
        std::cout << "Locations:" << std::endl;
        for(auto it : locations) {
            std::cout << it.first << "  ---  " << it.second << std::endl;
        }
    }
#endif

private:
    Locations(const Locations& orig);
    Locations& operator=(const Locations& orig);

    // key is location, value is its path
    std::map<std::string, std::string> locations;
};
