#pragma once

#include <Request.hpp>
#include <FileFuncs.hpp>

class RequestFabric {
public:
    RequestFabric() {}

    RequestInterface* create(char* requestBody, Locations* locations, int clientFd);
private:
};
