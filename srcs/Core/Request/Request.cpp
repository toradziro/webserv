#include <Request.hpp>
#include <FileFuncs.hpp>

static bool isPackageFull() {
    return true;
}

static void getLocationFromBody(const char* body, std::string& requestLocation) {
    int bodySize = strlen(body);
    int i = 0;
    requestLocation = "";
    // skip request type "GET "
    for(; i < bodySize; ++i) {
        if(body[i] == ' ')
            break;
    }

    // Set index on first simbol of location
    ++i;

    for(; i < bodySize; ++i) {
        // iterating till " HTTP/1.1"
        if(body[i] == ' ')
            break;
        requestLocation += body[i];
    }
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

static char* changePrefixWithLocation(const std::string& requestLocation, const std::string& locationToChange) {
#ifdef _DEBUG
    std::cout << "requestLocation IS: " << requestLocation << std::endl;
#endif
    // skip prefix
    char* res = (char*)calloc(requestLocation.size() + locationToChange.size(), sizeof(char));
    size_t i = 0;
    for(size_t j = 0; j < locationToChange.size(); ++j) {
        res[i] = locationToChange[j];
        ++i;
    }
    size_t startIndex = 1;
    for(; startIndex < requestLocation.size(); ++startIndex) {
        if(requestLocation[startIndex] == '/') {
            break;
        }
    }
    for(; startIndex < requestLocation.size(); ++startIndex) {
        res[i] = requestLocation[startIndex];
        ++i;
    }
    return res;
}

// Interface realisations
RequestGET::RequestGET(Locations* locations, ContentTypeCollection* contentTypes, char* requestBody, int clientFd) :
        m_Response(nullptr),
        m_locations(locations),
        m_contentTypes(contentTypes),
        m_requestBody(requestBody),
        m_requestLocation(nullptr),
        m_clientFd(clientFd)
{
    splitBody();
    free(m_requestBody);
    m_requestBody = nullptr;
}

int RequestGET::handleRequest() {
    std::string requestLocation = "";
    std::string locationPrefix = "";
    getLocationFromBody(m_splitBody[0].c_str(), requestLocation);
    getLocationPrefix(requestLocation, locationPrefix);
    if(m_locations->hasLocation(locationPrefix)) {
        std::string locationToChange = m_locations->getRoot(locationPrefix);
        m_requestLocation = changePrefixWithLocation(requestLocation, locationToChange);
    } else {
        m_requestLocation = strdup(requestLocation.c_str());
    }
    if(strstr(m_requestLocation, "..") != NULL) {
        deleteExtraPath();
    }
    // TODO: Add ".." deletion
#ifdef _DEBUG
    std::cout << "FINAL LOCATION IS: " << m_requestLocation << std::endl;
#endif

    if(isPackageFull()) {
        fillAndProcessResponse();
        return 0;
    }
    return -1;
}

void RequestGET::fillAndProcessResponse() {
    ResponseInfo info;

    info.m_clientFd = m_clientFd;
    info.m_contentTypes = m_contentTypes;
    info.m_hostPort = parseContent("Host: ");
    // std::cout << "\n\n\nHOST PARSED IS: " << info.m_hostPort << std::endl;
    info.m_requestLocation = m_requestLocation;

    m_Response = createResponse(info, RESP_GET);
    m_Response->prepareResponce();
    m_Response->sendResponse();
    delete m_Response;
}

std::string RequestGET::parseContent(std::string contetTag) {
    std::string res = "";
    for(size_t i = 0; i < m_splitBody.size(); ++i) {
        if(m_splitBody[i].find(contetTag) != std::string::npos) {
            for(size_t j = contetTag.size(); j < m_splitBody[i].size(); ++j) {
                res += m_splitBody[i][j];
            }
            break;
        }
    }
    std::cout << "PARSE CONTENT: " << res << std::endl;
    return res;
}

void RequestGET::splitBody() {
    size_t length = strlen(m_requestBody);
    for(size_t i = 0; i < length; ++i) {
        std::string tmp = "";
        while(i < length && m_requestBody[i] != '\n' && m_requestBody[i] != '\r') {
            tmp += m_requestBody[i];
            ++i;
        }
        if(tmp != "") {
            m_splitBody.push_back(tmp);
        }
    }
}

void RequestGET::deleteExtraPath() {
    std::vector<std::string> tokens;
    size_t length = strlen(m_requestLocation);
    for(size_t i = 0; i < length; ++i) {
        std::string tmp = "";
        while(i < length && m_requestLocation[i] != '/') {
            tmp += m_requestLocation[i];
            ++i;
        }
        tokens.push_back(tmp);
    }

    for(size_t i = 0; i < tokens.size(); ++i) {
        if(tokens[i] == ".." && i > 1) {
            tokens.erase(tokens.begin() + i);
            tokens.erase(tokens.begin() + (i - 1));
        }
    }

    std::string tmpRes = "";

    for(size_t i = 0; i < tokens.size(); ++i) {
        tmpRes += "/";
        tmpRes += tokens[i]; 
    }

    free(m_requestLocation);
    m_requestLocation = strdup(tmpRes.c_str());
}
