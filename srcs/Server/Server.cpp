#include <Server.hpp>

void Server::setListenPort(uint16_t port) {
    m_listenPort = port;
}

void Server::setIpAddress(const std::string& ipAddress) {
    m_ipAddress = std::move(ipAddress);
}

void Server::setServerName(const std::string& serverName) {
    m_serverName = std::move(serverName);
}

void Server::addLocation(const std::string& locationName, const std::string& locationRoot) {
    m_locations.addLocation(locationName, locationRoot);
}

#ifdef _DEBUG
void Server::printServer() {
    std::cout << "::::SERVER DATA::::" << std::endl;
    std::cout << "ip addres: " << m_ipAddress << std::endl;
    std::cout << "port: " << m_listenPort << std::endl;
    std::cout << "server_name: " << m_serverName << std::endl;
    m_locations.printLocations();
    std::cout << "::::SERVER DATA END::::" << std::endl;
}
#endif