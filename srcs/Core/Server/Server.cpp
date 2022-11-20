#include <Server.hpp>
#include <Selector.hpp>

extern std::sig_atomic_t gSignalStatus;

Server::Server() : m_ipAddress(""), m_serverName(""), m_epollFd(-1),
    m_serverSocket(-1), m_listenPort(0), m_isRunning(false)
{
}

void Server::setListenPort(uint16_t port) {
    m_listenPort = port;
}

void Server::setIpAddress(const std::string& ipAddress) {
    m_ipAddress = std::move(ipAddress);
}

void Server::setServerName(const std::string& serverName) {
    m_serverName = std::move(serverName);
}

void Server::setContentTypeCollection(ContentTypeCollection* contentTypes) {
    m_contentTypes = contentTypes;
}

void Server::addLocation(const std::string& locationName, const std::string& locationRoot) {
    m_locations.addLocation(locationName, locationRoot);
}

void Server::checkServerInstance() {
    checkError(m_locations.checkIsEmpty(), "no locations in configuration");
    checkError(m_ipAddress == "", "no ip to listen listet is set");
}

void Server::prepareForStart() {
    // Create socket
    m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    checkError(m_serverSocket == -1, "server socket creation");

    // Set the socket up
    sockaddr_in socketAddress;
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(m_listenPort);
    inet_pton(AF_INET, m_ipAddress.c_str(), &socketAddress.sin_addr);
    
    // Bind socket
    checkError(bind(m_serverSocket, (sockaddr*)&socketAddress, sizeof(socketAddress)) == -1,
        "cannot bind server socket");
    
    // Set socket to listen
    checkError(listen(m_serverSocket, SOMAXCONN) == -1,
        "server doesn't want to listen, hard to jugje we all know what asshole written him");
    checkError(fcntl(m_serverSocket, F_SETFL, O_NONBLOCK) == -1, 
        "server wants to work only in blocking mode");

    // Creating event selector
    m_epollFd = epoll_create(EVENTS_NUM);
    checkError(m_epollFd == -1, "couldn't create epoll instance")

    // Set up epoll in "Level Trigger" mode
    // m_epollEvent.events = EPOLLIN;
    m_epollEvent.events = EPOLLIN;
    m_epollEvent.data.fd = m_serverSocket;
    checkError(epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_serverSocket, &m_epollEvent) == -1,
        "epoll_ctl didn't work");
    std::cout << "Succesfully prepared server" << std::endl;
}

void Server::start() {
    m_isRunning = true;
    Selector selector(m_serverSocket, m_epollFd, &m_locations, m_contentTypes);
    while(m_isRunning) {
        if(gSignalStatus != 0) {
            m_isRunning = false;
        }
        selector.run();
    }
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
