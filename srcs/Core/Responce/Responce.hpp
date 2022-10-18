#pragma once

#include <common_inc.h>

class Responce {
public:
    Responce(int clientFd);
    void sendResponce();

private:
    int m_clientFd;
    int m_responceCode;
    char* m_responceBody;
};