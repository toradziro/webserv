#include <RequestFabric.hpp>

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

RequestInterface* RequestFabric::create(char* requestBody, Locations* locations,
                            ContentTypeCollection* contentTypes, int clientFd) {
    RequestInterface* request = nullptr;
    std::cout << requestBody << std::endl;
    switch (findRequestType(requestBody))
    {
    case RT_GET:
        request = new RequestGET(locations, contentTypes, requestBody, clientFd);
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