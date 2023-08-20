#include <ClientHandler.hpp>
#include <sstream>

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

ClientHandler::ClientHandler(std::string serverRoot, Locations* locations, int clientFd) :
                            m_serverRoot(serverRoot),
                            m_locations(locations),
                            m_clientFd(clientFd)
{ }

void ClientHandler::HandleRequest() {
    readRequest();
    RequestConfig requestConfig = std::move(ClientHandler::parseRequest());
#ifdef _DEBUG
    std::cout << "------location: " << requestConfig.m_location << std::endl;
    std::cout << "------body: " << requestConfig.m_body << std::endl;
    requestConfig.entityHeaderTable.debugPrint();
    requestConfig.generalHeaderTable.debugPrint();
    requestConfig.requestHeaderTable.debugPrint();
#endif
}

RequestConfig ClientHandler::parseRequest() {
    RequestConfig requestConfig;
    separateSections();
    std::stringstream headerStream(m_header);
    std::string methodName;
    headerStream >> methodName >> requestConfig.m_location;
    requestConfig.m_reqType = parseRequestType(methodName);
    parseAndReplaceLocation(requestConfig.m_location);
    parseHeadersAndBody(requestConfig);
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

void ClientHandler::parseAndReplaceLocation(std::string& requestLocation) {
    // getting location prefix
    std::stringstream requestLocationStream(requestLocation);
    std::string prefix;
    std::getline(requestLocationStream, prefix, '/');

    if(m_locations->hasLocation(prefix)) {
        std::string finalLocation = m_locations->getRoot(prefix);
        while(std::getline(requestLocationStream, prefix, '/')) {
            finalLocation += "/";
            finalLocation += prefix;
        }
        requestLocation = std::move(finalLocation);
    }
}

void ClientHandler::readRequest() {
    char* clientMessage = (char*)calloc(messageLenght, sizeof(char));
    checkError(clientMessage == NULL, "didn't allocated space for client");
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
    std::cout << "m_header ----- " << m_header << std::endl;
    readSection(m_readMessage, i, m_extraHeaders);
    std::cout << "m_extraHeaders ----- " << m_extraHeaders << std::endl;
    readSection(m_readMessage, i, m_body);
    std::cout << "m_body ----- " << m_body << std::endl;
}
