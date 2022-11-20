#pragma once

#include <common_inc.h>

enum ResponseType {
    RESP_GET,
    RESP_POST,

    RESP_UNKNOWN,
};

class ResponseInterface {
public:
    virtual ~ResponseInterface() = 0;
    virtual void sendResponse() = 0;
};
