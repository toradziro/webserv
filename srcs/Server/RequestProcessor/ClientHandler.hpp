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

    ClientHandler(ClientHandler&& another) {
        m_body = std::move(another.m_body);
        m_readMessage = std::move(another.m_readMessage);
        m_header = std::move(another.m_header);
        m_extraHeaders = std::move(another.m_extraHeaders);
        m_serverRoot = std::move(another.m_serverRoot);
        m_CGILocation = std::move(another.m_CGILocation);
        m_serverName = std::move(another.m_serverName);
        m_contentTypes = another.m_contentTypes;
        m_locations = another.m_locations;
        m_allowedCGI = another.m_allowedCGI;
        m_clientFd = another.m_clientFd;

        another.m_contentTypes = nullptr;
        another.m_locations = nullptr;
        another.m_allowedCGI = nullptr;
        another.m_clientFd = -1;
    }

    ClientHandler& operator=(ClientHandler&& another) {
        if(&another != this) {
            m_body = std::move(another.m_body);
            m_readMessage = std::move(another.m_readMessage);
            m_header = std::move(another.m_header);
            m_extraHeaders = std::move(another.m_extraHeaders);
            m_serverRoot = std::move(another.m_serverRoot);
            m_CGILocation = std::move(another.m_CGILocation);
            m_serverName = std::move(another.m_serverName);
            m_contentTypes = another.m_contentTypes;
            m_locations = another.m_locations;
            m_allowedCGI = another.m_allowedCGI;
            m_clientFd = another.m_clientFd;

            another.m_contentTypes = nullptr;
            another.m_locations = nullptr;
            another.m_allowedCGI = nullptr;
            another.m_clientFd = -1;
        }
        return *this;
    }

    ClientHandler(const ClientHandler&) = delete;
    ClientHandler operator=(const ClientHandler&) = delete;
    ClientHandler() = delete;

    void setThreadId(int threadId) {
        m_threadId = threadId;
    }

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
    int                     m_threadId = -1;
};
