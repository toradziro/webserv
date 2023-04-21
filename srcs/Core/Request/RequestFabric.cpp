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
                            ContentTypeCollection* contentTypes, int clientFd,
                            const std::string& serverRoot)
{
    RequestInterface* request = nullptr;
    std::cout << requestBody << std::endl;
    switch (findRequestType(requestBody))
    {
    case RT_GET:
        request = new RequestGET(locations, contentTypes, requestBody, clientFd, serverRoot);
        break;

    case RT_POST:
        request = new RequestPOST(locations, requestBody, clientFd);
        break;

    case RT_UNKNOWN:
        printf("UNKNOWN:: %s\n", requestBody);
        // TODO: change to responce "unknown request"
        assert(false);
        break;

    default:
        break;
    }
    return request;
}