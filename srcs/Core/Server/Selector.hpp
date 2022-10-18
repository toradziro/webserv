#pragma once

#include <common_inc.h>
#include <RequestCollection.hpp>
#include <Locations.hpp>

#define EVENTS_NUM  1024
#define INFINITE    -1

class Selector {
public:
    explicit Selector(int serverSocket, int epollSocket, Locations* locations);
    void run();

private:
    int         m_serverSocket;
    int         m_epollSocket;
    Locations*  m_locations;
};
