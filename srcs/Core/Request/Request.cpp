#include <Request.hpp>
#include <FileFuncs.hpp>

const std::string page404 = "./test_files/404_not_found.html"; 

static bool isPackageFull() {
    return true;
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

static std::string getContentType(std::string fileName) {
    std::string contentType = "";
    std::string fileExtention = getFileExtention(fileName);
    if(fileExtention == "html") {
        contentType = "text/html";
    } else if(fileExtention == "bmp") {
        contentType = "image/bmp";
    } else if(fileExtention == "jpeg" || fileExtention == "jpg") {
        contentType = "";
    } else {
        contentType = "text/plain";
    }
    return contentType;
}

// Interface realisations
RequestGET::RequestGET(Locations* locations, char* requestBody, int clientFd) :
        m_Response(nullptr),
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
        fillResponse();
        return 0;
    }
    return -1;
}

void RequestGET::fillResponse() {
    std::string ResponseCode = "";
    std::string contentType = "";
    int fileFd = 0;
    ResponseNum ResponseNum;

    if(isFileExists(m_requestLocation)) {
        ResponseNum = RN_200;
    } else {
        ResponseNum = RN_404;
    }

    if(ResponseNum == RN_200) {
        ResponseCode = "200 OK";
        std::cout << "REQUEST_LOCATION: " << m_requestLocation << std::endl;
        contentType = getContentType(m_requestLocation);
        fileFd = open(m_requestLocation, O_RDONLY);
    } else if(ResponseNum == RN_404) {
        ResponseCode = "404 Not Found";
        contentType = "text/html";
        fileFd = open(page404.c_str(), O_RDONLY);
    } else {
        assert(false);
    }

    m_Response = new Response(m_clientFd,
                    std::move(ResponseCode),
                    std::move(contentType),
                    fileFd);

    m_Response->sendResponse();
    delete m_Response;
}
