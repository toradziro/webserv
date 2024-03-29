#include <ClientHandler.hpp>
#include <sstream>
#include <RequestHandler.hpp>
#include <GetRequestHandler.hpp>
#include <PostRequestHandler.hpp>
#include <RequestHandlerFabric.hpp>
#include <Log.hpp>

/*
GET /pub/WWW/TheProject.html HTTP/1.1
Host: www.w3.org
*/

/*
POST /test HTTP/1.1
Host: foo.example
Content-Type: application/x-www-form-urlencoded
Content-Length: 27

field1=value1&field2=value2
*/

ClientHandler::ClientHandler(const std::string& serverRoot, const std::string& CGILocation, const std::string& serverName, 
                Locations* locations, ContentTypeCollection* contentTypes, AllowedCGIExecutors* allowedCGI, int clientFd) :
                m_serverRoot(serverRoot),
                m_CGILocation(CGILocation),
                m_serverName(serverName),
                m_contentTypes(contentTypes),
                m_locations(locations),
                m_allowedCGI(allowedCGI),
                m_clientFd(clientFd)
{ }

void ClientHandler::HandleRequest() {
    readRequest();
    if(m_readMessage == "") {
        return;
    }
    RequestConfig requestConfig = std::move(ClientHandler::parseRequest());
    
#ifdef _DEBUG
    std::cout << "------location: " << requestConfig.m_location << std::endl;
    std::cout << "------body: " << requestConfig.m_body << std::endl;
    requestConfig.entityHeaderTable.debugPrint();
    requestConfig.generalHeaderTable.debugPrint();
    requestConfig.requestHeaderTable.debugPrint();
#endif
    LogMessage("Worker thread number: " + std::to_string(m_threadId) + " takes " + 
                requestConfig.m_requestStringName + " request from client: " + std::to_string(m_clientFd));
    std::shared_ptr<RequestHandler> requestHandler = createRequestHandler(std::move(requestConfig));
    try {
        requestHandler->prepareResponce();
        requestHandler->sendResponse();
    } catch(CException* e) {
        LogMessage(e->getDescription());
        delete e;
    }
}

RequestConfig ClientHandler::parseRequest() {
    RequestConfig requestConfig;
    separateSections();
    std::stringstream headerStream(m_header);
    std::string methodName;
    headerStream >> methodName >> requestConfig.m_location;

    requestConfig.m_reqType = parseRequestType(methodName);
    requestConfig.m_requestStringName = std::move(methodName);
    parseAndReplaceLocation(requestConfig.m_location, requestConfig);
    parseHeadersAndBody(requestConfig);
    fillConfigWithCommonInfo(requestConfig);
    
    return requestConfig;
}

RequestType ClientHandler::parseRequestType(const std::string& stringType) {
    if(stringType == "GET") {
        return RequestType::RT_GET;
    } else if(stringType == "POST") {
        return RequestType::RT_POST;
    } else if(stringType == "PUT") {
        return RequestType::RT_PUT;
    }
    return RequestType::RT_UNKNOWN;
}

static bool isAHeaderKey(const std::string& token, const RequestConfig& config) {
    return config.generalHeaderTable.Has(token) || 
            config.requestHeaderTable.Has(token) ||
            config.entityHeaderTable.Has(token);
}

void ClientHandler::parseHeadersAndBody(RequestConfig& config) {
    std::stringstream extraHeadersStream(m_extraHeaders);
    std::string token;
    std::string currHeader;

    while(true) {
        if(isAHeaderKey(token, config)) {
            currHeader = token;
        } else {
            if(extraHeadersStream >> token) {
                continue;
            }
            break;
        }
        
        std::string value;
        while(true) {
            if(!(extraHeadersStream >> token) || isAHeaderKey(token, config)) {
                break;
            }
            value += token;
        }

        if(config.generalHeaderTable.Has(currHeader)) {
            config.generalHeaderTable.AddValue(currHeader, value);
        } else if(config.requestHeaderTable.Has(currHeader)) {
            config.requestHeaderTable.AddValue(currHeader, value);
        } else if(config.entityHeaderTable.Has(currHeader)) {
            config.entityHeaderTable.AddValue(currHeader, value);
        }
    }
}

void ClientHandler::fillConfigWithCommonInfo(RequestConfig& config) {
    config.m_body = std::move(m_body);
    config.m_clientFd = m_clientFd;
    config.m_serverRoot = std::move(m_serverRoot);
    config.m_contentTypes = m_contentTypes;
    config.m_allowedCGI = m_allowedCGI;
    config.m_CGILocation = std::move(m_CGILocation);
    config.m_serverName = std::move(m_serverName);
}


static void getLocationPrefix(const std::string& requestLocation, std::string& locationPrefix) {
    locationPrefix = "";
    // first simbol is always '/'
    locationPrefix += requestLocation[0];
    for(size_t i = 1; i < requestLocation.size(); ++i) {
        if(requestLocation[i] == '/') {
            break;
        }
        locationPrefix += requestLocation[i];
    }
}

static std::string changePrefixWithLocation(const std::string& requestLocation, const std::string& locationToChange) {
    // skip prefix
    std::string res;
    size_t i = 0;
    for(size_t j = 0; j < locationToChange.size(); ++j) {
        res += locationToChange[j];
        ++i;
    }
    size_t startIndex = 1;
    for(; startIndex < requestLocation.size(); ++startIndex) {
        if(requestLocation[startIndex] == '/') {
            break;
        }
    }
    for(; startIndex < requestLocation.size(); ++startIndex) {
        // res[i] = requestLocation[startIndex];
        res += requestLocation[startIndex];
        ++i;
    }
    return res;
}

[[nodiscard]]static inline size_t checkContainsQueryString(const std::string& location) {
    return location.find('?');
}

void ClientHandler::parseAndReplaceLocation(std::string& requestLocation, RequestConfig& config) {
    // check if contains query string
    size_t queryStringPos = checkContainsQueryString(requestLocation);
    if(queryStringPos != requestLocation.npos) {
        config.m_queryString = requestLocation.substr(queryStringPos + 1);
        requestLocation.resize(queryStringPos);
    }
    // getting location prefix
    std::string locationPrefix = "";
    getLocationPrefix(requestLocation, locationPrefix);
    if(m_locations->hasLocation(locationPrefix)) {
        std::string locationToChange = m_locations->getRoot(locationPrefix);
        requestLocation = changePrefixWithLocation(requestLocation, locationToChange);
    }
}

void ClientHandler::readRequest() {
    char* clientMessage = (char*)calloc(messageLenght, sizeof(char));
    if(clientMessage == nullptr) {
        checkError(true, "didn't allocated space for client");
        return;
    }
    int wasRecived = 0;

    while(true) {
        wasRecived = recv(m_clientFd, clientMessage, messageLenght, 0);
        checkError(wasRecived == -1, "error occured during reading client message");

        if(wasRecived < messageLenght) {
            for(int i = 0; i < wasRecived; ++i) {
                m_readMessage += clientMessage[i];
            }
            break;
        } else {
            m_readMessage += std::string(clientMessage);
        }
    }
    free(clientMessage);
}

static void readSection(const std::string& readMessage, size_t& i, std::string& section) {
    while(i < readMessage.size()) {
        if(readMessage[i] == '\r' && readMessage[i + 1] == '\n'
            && readMessage[i + 2] == '\r' && readMessage[i + 3] == '\n') {
            break;
        }
        if(readMessage[i] == '\r') {
            ++i;
            continue;
        }
        section += readMessage[i];
        ++i;
    }
    // step on the first char after \r\n
    i += 4;
}

void ClientHandler::separateSections() {
    size_t i = 0;
    while(i < m_readMessage.size()) {
        if(m_readMessage[i] == '\r' && m_readMessage[i + 1] == '\n')
            break;
        m_header += m_readMessage[i];
        ++i;
    }
    i += 2;
    readSection(m_readMessage, i, m_extraHeaders);
    readSection(m_readMessage, i, m_body);
#ifdef _DEBUG
    std::cout << m_header << std::endl << m_extraHeaders << std::endl << m_body << std::endl; 
#endif
}
