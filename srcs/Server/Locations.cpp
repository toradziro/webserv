#include <Locations.hpp>

bool Locations::hasLocation(const std::string& locationName) {
    if(locations.count(locationName) == 0) {
        return false;
    }
    return true;
}

    // return path 
std::string Locations::getRoot(const std::string& locationName) {
    return locations[locationName];
}

void Locations::addLocation(const std::string& locationName, const std::string& rootPath) {
    locations[locationName] = rootPath;
}

bool Locations::checkIsEmpty() {
    return locations.empty();
}

#ifdef _DEBUG
void Locations::printLocations() {
    std::cout << "Locations:" << std::endl;
    for(auto it : locations) {
        std::cout << "LABEL: " << it.first << "  ==  " << "ROOT: " << it.second << std::endl;
    }
}
#endif