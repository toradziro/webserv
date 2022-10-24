#include <Request.hpp>
#include <FileFuncs.hpp>

const std::string getRequestString = "GET";
const std::string postRequestString = "POST";

const std::string page404 = "./test_files/404_not_found.html"; 

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

static void getLocationFromBody(char* body, std::string& requestLocation) {
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
RequestGET::RequestGET(Locations* locations, char* requestBody, int clientFd) :
        m_responce(nullptr),
        m_locations(locations),
        m_requestBody(requestBody),
        m_requestLocation(nullptr),
        m_clientFd(clientFd)
{
}

int RequestGET::handleRequest() {
    std::string requestLocation = "";
    std::string locationPrefix = "";
    getLocationFromBody(m_requestBody, requestLocation);
    getLocationPrefix(requestLocation, locationPrefix);
    if(m_locations->hasLocation(locationPrefix)) {
        std::string locationToChange = m_locations->getRoot(locationPrefix);
        m_requestLocation = changePrefixWithLocation(requestLocation, locationToChange);
    } else {
        m_requestLocation = strdup(requestLocation.c_str());
    }

#ifdef _DEBUG
    std::cout << "FINAL LOCATION IS: " << m_requestLocation << std::endl;
#endif

    if(isPackageFull()) {
        fillResponce();
        return 0;
    }
    return -1;
}

void RequestGET::fillResponce() {
    std::string responceCode = "";
    std::string contentType = "";
    int fileFd = 0;
    ResponceNum responceNum;
    if(isFileExists(m_requestLocation)) {
        responceNum = RN_200;
    } else {
        responceNum = RN_404;
    }

    if(responceNum == RN_200) {
        responceCode = "200 OK";
        std::string fileExtention = getFileExtention(m_requestLocation);
        if(fileExtention == "html") {
            contentType = "text/html";
        } else {
            contentType = "text/plain";
        }
        fileFd = open(m_requestLocation, O_RDONLY);
    } else if(responceNum == RN_404) {
        responceCode = "404 Not Found";
        contentType = "text/html";
        fileFd = open(page404.c_str(), O_RDONLY);
    } else {
        assert(false);
    }

    checkError(fileFd == -1, "can't open file for responce");
    int fileSize = getFileSize(fileFd);
    char* fileMapping = (char*)mmap(NULL,
                            fileSize,
                            PROT_READ,
                            MAP_PRIVATE,
                            fileFd, 0);

    checkError(fileMapping == NULL, "responce mapping failed");
    std::string responceBody(fileMapping);
    checkError(munmap((void*)fileMapping, fileSize) == -1, "responce unmapping failed");

    m_responce = new Responce(m_clientFd,
                    fileSize,
                    std::move(responceCode),
                    std::move(contentType),
                    std::move(responceBody));

    m_responce->sendResponce();
    delete m_responce;
}
