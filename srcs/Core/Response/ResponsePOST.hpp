#pragma once

#include <common_inc.h>
#include <ResponseInterface.hpp>

class ResponsePOST : public ResponseInterface {
public:
    ResponsePOST() {}
    void sendResponse() { assert(false); }
    void prepareResponce(){ assert(false); }
    bool isValid() { assert(false); return false; }
};