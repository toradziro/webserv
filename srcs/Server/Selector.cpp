#include <Selector.hpp>
#include <ContentTypeCollection.hpp>
#include <ClientHandler.hpp>

static void debug_epoll_event(epoll_event ev){
       printf("fd(%d), ev.events:", ev.data.fd);

       if(ev.events & EPOLLIN)
               printf(" EPOLLIN ");
       if(ev.events & EPOLLOUT)
               printf(" EPOLLOUT ");
       if(ev.events & EPOLLET)
               printf(" EPOLLET ");
       if(ev.events & EPOLLPRI)
               printf(" EPOLLPRI ");
       if(ev.events & EPOLLRDNORM)
               printf(" EPOLLRDNORM ");
       if(ev.events & EPOLLRDBAND)
               printf(" EPOLLRDBAND ");
       if(ev.events & EPOLLWRNORM)
               printf(" EPOLLRDNORM ");
       if(ev.events & EPOLLWRBAND)
               printf(" EPOLLWRBAND ");
       if(ev.events & EPOLLMSG)
               printf(" EPOLLMSG ");
       if(ev.events & EPOLLERR)
               printf(" EPOLLERR ");
       if(ev.events & EPOLLHUP)
               printf(" EPOLLHUP ");
       if(ev.events & EPOLLONESHOT)
               printf(" EPOLLONESHOT ");

       printf("\n");

}

Selector::Selector(int serverSocket, int epollSocket,
                    Locations* locations,
                    ContentTypeCollection* contentTypeCollection,
                    AllowedCGIExecutors* allowedCGI,
                    const std::string& serverRoot,
                    const std::string& CGILocation,
                    const std::string& serverName) :
    m_events{},
    m_serverRoot(serverRoot),
    m_CGILocation(CGILocation),
    m_serverName(serverName),
    m_contentTypes(contentTypeCollection),
    m_locations(locations), 
    m_allowedCGI(allowedCGI),
    m_serverSocket(serverSocket),
    m_epollSocket(epollSocket)
{
}

void Selector::run()
{
    int eventsOccuredNum = epoll_wait(m_epollSocket, m_events, EVENTS_NUM, INFINITE);
    if(eventsOccuredNum == -1) {
        checkError(true, "epoll interface got broken on wait");
        std::terminate();
    }
    for(int i = 0; i < eventsOccuredNum; ++i) {
        debug_epoll_event(m_events[i]);
        if(m_events[i].data.fd == m_serverSocket) {
            // new connection
            sockaddr_in clientAddress;
            socklen_t clientAddressSize = sizeof(clientAddress);
            struct epoll_event clientEvent;
            int clientSocket = accept(m_serverSocket, ( sockaddr* )&clientAddress, &clientAddressSize);
            checkError(clientSocket == -1, "can't accept client");
            checkError(fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1, 
                "client wants to work only in blocking mode");
            clientEvent.data.fd = clientSocket;
            clientEvent.events = EPOLLIN | EPOLLET;
            checkError(epoll_ctl(m_epollSocket, EPOLL_CTL_ADD, clientSocket, &clientEvent) == -1,
                "addidng client to epoll");
        } else {
            // msg from previosly traced connection
            ClientHandler clientHandler(m_serverRoot, m_CGILocation, m_serverName, m_locations, m_contentTypes, m_allowedCGI, m_events[i].data.fd);
            clientHandler.HandleRequest();
        }
    }
}
