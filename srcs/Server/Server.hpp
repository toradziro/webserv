#pragma once

#include <common_inc.h>
#include <Locations.hpp>

class Server {
public:
    Server() : ipAddress(""), listenPort(0) { }
    ~Server() { }

    void setListenPort(uint16_t port) {
        listenPort = port;
    }

    void setIpAddress(const std::string& _ipAddress) {
        ipAddress = _ipAddress;
    }

    void addLocation(const std::string& locationName, const std::string& locationRoot) {
        locations.addLocation(locationName, locationRoot);
    }

#ifdef _DEBUG
    void printServer() {
        std::cout << "ip addres: " << ipAddress << std::endl;
        std::cout << "port: " << listenPort << std::endl;
        locations.printLocations();
    }
#endif

private:
    Server(const Server& other);
    Server& operator=(const Server& other);

    Locations locations;
    std::string ipAddress;
    uint16_t listenPort;
};