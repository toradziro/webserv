#include <Response.hpp>
#include <FileFuncs.hpp>
#include <ResponseGET.hpp>
#include <ResponsePOST.hpp>

ResponseInterface* createResponse(ResponseInfo info, ResponseType responseType) {
    switch(responseType) {
        case RESP_GET:
            return new ResponseGET(info); 
            break;
        case RESP_POST:
            break;
        case RESP_UNKNOWN:
            return new ResponsePOST();
        default:
            // should never be here TODO: change assert to log handler
            assert(false);
    }
    return nullptr;
}
