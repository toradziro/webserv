#pragma once

#include <common_inc.h>
#include <CommonResponseSender.hpp>

enum class ErrorCode {
    EC_404,
    EC_403,
    EC_500,
};

void sendError(ErrorCode errorCode, int clientFd);

class Error404Sender {
protected:
    Error404Sender() {}
    ~Error404Sender() {}

public:
    Error404Sender(Error404Sender&) = delete;
    Error404Sender operator=(const Error404Sender&) = delete;
    
    static Error404Sender* GetInstance();
    void SendError(int clientFd);

private:
    static Error404Sender* m_instance;
    static std::mutex m_mutex;
};

class Error403Sender {
protected:
    Error403Sender() {}
    ~Error403Sender() {}

public:
    Error403Sender(Error403Sender&) = delete;
    Error403Sender operator=(const Error403Sender&) = delete;
    
    static Error403Sender* GetInstance();
    void SendError(int clientFd);

private:
    static Error403Sender* m_instance;
    static std::mutex m_mutex;
};

class Error500Sender {
protected:
    Error500Sender() {}
    ~Error500Sender() {}
public:
    Error500Sender(Error500Sender&) = delete;
    Error500Sender operator=(const Error500Sender&) = delete;
    
    static Error500Sender* GetInstance();
    void SendError(int clientFd);
private:
    static Error500Sender* m_instance;
    static std::mutex m_mutex;
};
