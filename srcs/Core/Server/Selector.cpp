#include <Selector.hpp>
#include <Request.hpp>
#include <RequestCollection.hpp>

Selector::Selector(int serverSocket, int epollSocket, Locations* locations) :
   m_serverSocket(serverSocket), m_epollSocket(epollSocket), m_locations(locations) { }


void Selector::run()
    {
        RequestCollection requests;
        struct epoll_event serverEvents[EVENTS_NUM];
        int eventsOccuredNum = epoll_wait(m_epollSocket, serverEvents, EVENTS_NUM, INFINITE);
        if(eventsOccuredNum == -1) {
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
                checkError(epoll_ctl(m_epollSocket, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1,
                    "addidng client to epoll");
            } else {
                // msg from previosly traced connection
                requests.processMessage(serverEvents[i].data.fd);
            }
        }
    }