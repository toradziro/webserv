#include <Server.hpp>

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
    m_epollEvent.events = EPOLLIN;
    m_epollEvent.data.fd = m_serverSocket;
    checkError(epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_serverSocket, &m_epollEvent) == -1,
        "epoll_ctl didn't work");
    std::cout << "Succesfully prepared server" << std::endl;
}

void Server::start() {
    struct epoll_event serverEvents[EVENTS_NUM];
    m_isRunning = true;
    while(m_isRunning) {
        int eventsOccuredNum = epoll_wait(m_epollFd, serverEvents, EVENTS_NUM, INFINITE);
        if( eventsOccuredNum == -1 ) {
            checkError(eventsOccuredNum == -1, "epoll interface got broken on wait");
        }
        for(int i = 0; i < eventsOccuredNum; ++i) {
            if(serverEvents[i].data.fd == m_serverSocket) {
                // new connection
                sockaddr_in clientAddress;
                socklen_t clientAddressSize = sizeof(clientAddress);
                struct epoll_event clientEvent;
                int clientSocket = accept(m_serverSocket, ( sockaddr* )&clientAddress, &clientAddressSize);
                checkError(clientSocket == -1, "can't accept client");
                checkError(fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1, 
                    "client wants to work only in blocking mode");
                clientEvent.data.fd = clientSocket;
                clientEvent.events = EPOLLIN;
                checkError(epoll_ctl(m_epollFd, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1,
                    "addidng client to epoll");
#ifdef _DEBUG
                char clientMessage[10000] = {0};
                int messageLength = read(clientSocket, clientMessage, 10000);
                checkError(messageLength == -1, "tried to receive msg from new client");
                printf("Got message \"%s\" from client with fd == %d\n", clientMessage, clientSocket);
                const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
                write(clientSocket , hello , strlen(hello));
#endif
            } else {
                // msg from previosly traced connection
                int clientFd = serverEvents[i].data.fd;
                char clientMessage[1024];
                int messageLength = recv(clientFd, clientMessage, 1024, 0);
                checkError(messageLength == -1, "tried to receive msg from new client");
                printf("Got message \"%s\" from client with fd == %d\n", clientMessage, clientFd);
            }
        }
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
