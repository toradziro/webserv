#pragma once

#include <Request.hpp>
#include <FileFuncs.hpp>
#include <ContentTypeCollection.hpp>

class RequestFabric {
public:
    RequestFabric() {}

    RequestInterface* create(char* requestBody, Locations* locations,
                            ContentTypeCollection* contentTypes, int clientFd,
                            const std::string& serverRoot);
private:
};
