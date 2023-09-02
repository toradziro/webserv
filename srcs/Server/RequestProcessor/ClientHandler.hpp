#pragma once

#include <common_inc.h>
#include <Locations.hpp>
#include <RequestConfig.hpp>
#include <AllowedCGIExecutors.hpp>

const int messageLenght = 8192;

class ClientHandler {
public:
    ClientHandler(const std::string& serverRoot, const std::string& CGILocation, const std::string& serverName, 
                Locations* locations, ContentTypeCollection* contentTypes, AllowedCGIExecutors* allowedCGI, 
                int clientFd);

    void HandleRequest();

    ClientHandler(const ClientHandler&) = delete;
    ClientHandler operator=(const ClientHandler&) = delete;
    ClientHandler() = delete;

private:
    RequestConfig parseRequest();
    RequestType parseRequestType(const std::string& stringType);
    void parseHeadersAndBody(RequestConfig& config);
    void parseAndReplaceLocation(std::string& requestLocation, RequestConfig& config);
    void fillConfigWithCommonInfo(RequestConfig& config);
    void readRequest();
    void separateSections();
    
    std::string             m_body;
    std::string             m_readMessage;
    std::string             m_header;
    std::string             m_extraHeaders;
    std::string             m_serverRoot;
    std::string             m_CGILocation;
    std::string             m_serverName;
    ContentTypeCollection*  m_contentTypes;
    Locations*              m_locations;
    AllowedCGIExecutors*    m_allowedCGI;
    int                     m_clientFd;
};
