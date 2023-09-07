#include <ErrorSender.hpp>

std::string page404 = "./test_files/404_not_found.html";
const std::string Error500 = "500 Internal Server Error";
const std::string Error404 = "404 Not Found";
const std::string Error403 = "403 Forbidden";

std::mutex Error404Sender::m_mutex;
std::mutex Error403Sender::m_mutex;
std::mutex Error500Sender::m_mutex;

Error404Sender* Error404Sender::m_instance;
Error403Sender* Error403Sender::m_instance;
Error500Sender* Error500Sender::m_instance;

void sendError(ErrorCode errorCode, int clientFd) {
    switch(errorCode)
    {
        case ErrorCode::EC_404:
            Error404Sender::GetInstance()->SendError(clientFd);
            break;

        case ErrorCode::EC_403:
            Error403Sender::GetInstance()->SendError(clientFd);
            break;

        case ErrorCode::EC_500:
            Error500Sender::GetInstance()->SendError(clientFd);
            break;

        default:
            break;
    }
}

Error404Sender* Error404Sender::GetInstance() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_instance == nullptr) {
        m_instance = new Error404Sender; 
    }
    return m_instance;
}

void Error404Sender::SendError(int clientFd) {
    int fileFd = open(page404.c_str(), O_RDONLY);
    CommonResponseSender sender(clientFd, fileFd, Error404, "text/html", page404);
    sender.sendResponse();
}

Error403Sender* Error403Sender::GetInstance() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_instance == nullptr) {
        m_instance = new Error403Sender;
    }
    return m_instance;
}

void Error403Sender::SendError(int clientFd) {
    CommonResponseSender sender(clientFd, "", Error403);
    sender.sendResponse();
}
 
Error500Sender* Error500Sender::GetInstance() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if(m_instance == nullptr) {
        m_instance = new Error500Sender; 
    }
    return m_instance;
}

void Error500Sender::SendError(int clientFd) {
    CommonResponseSender sender(clientFd, "", Error403);
    sender.sendResponse();
}