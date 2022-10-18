#include <Request.hpp>

const std::string getRequestString = "GET";
const std::string postRequestString = "POST";

static RequestType findRequestType(const char* requestBody) {
    std::string type = "";
    while(*requestBody != ' ') {
        type += *requestBody;
        ++requestBody;
    }
    if(type == getRequestString) {
        return RT_GET;
    } else if(type == postRequestString) {
        return RT_POST;
    }
    return RT_UNKNOWN;
}

static bool isPackageFull() {
    return true;
}

RequestInterface* requestFabric(char* requestBody, Locations* locations, int clientFd) {
    RequestInterface* request = nullptr;
    switch (findRequestType(requestBody))
    {
    case RT_GET:
        request = new RequestGET(locations, requestBody, clientFd);
        break;
    
    case RT_POST:
        request = new RequestPOST(locations, requestBody, clientFd);
        break;

    case RT_UNKNOWN:
        printf("%s\n", requestBody);
        assert(false);
        break;

    default:
        break;
    }
    return request;
}

// Interface realisations
RequestGET::RequestGET(Locations* locations, char* requestBody, int clientFd) :
        m_responce(nullptr),
        m_locations(locations),
        m_requestBody(requestBody),
        m_clientFd(clientFd)
{
}

int RequestGET::handleRequest() {
    if(isPackageFull()) {
        fillResponce();
        return 0;
    }
    return -1;
}

void RequestGET::fillResponce() {
    m_responce = new Responce(m_clientFd);
    m_responce->sendResponce();
    delete m_responce;
}
