#pragma once

#include <common_inc.h>
#include <Locations.hpp>
#include <RequestConfig.hpp>

const int messageLenght = 8192;

class ClientHandler {
public:
    ClientHandler(std::string serverRoot, Locations* m_locations, int clientFd);
    void HandleRequest();

    ClientHandler(const ClientHandler&) = delete;
    ClientHandler operator=(const ClientHandler&) = delete;
    ClientHandler() = delete;
private:
    RequestConfig parseRequest();
    RequestType parseRequestType(const std::string& stringType);
    void parseHeadersAndBody(RequestConfig& config);
    void parseAndReplaceLocation(std::string& requestLocation);
    void readRequest();
    void separateSections();
    
    std::string m_body;
    std::string m_readMessage;
    std::string m_header;
    std::string m_extraHeaders;
    std::string m_serverRoot;
    Locations*  m_locations;
    int         m_clientFd;
};
