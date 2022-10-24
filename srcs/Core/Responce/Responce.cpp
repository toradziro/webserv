#include <Responce.hpp>

// "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"
// 404 Not Found
// 200 OK
// text/html

std::string httpType = "HTTP/1.1";

Responce::Responce(int clientFd,
            int contentLength,
            std::string responceCode,
            std::string contentType,
            std::string responceBody) :
    
            m_responceCode(responceCode),
            m_contentType(contentType),
            m_responceBody(responceBody),
            m_clientFd(clientFd),
            m_contentLength(contentLength)
{
} 

void Responce::sendResponce() {
    std::string resultMessage = "";
    resultMessage += httpType + " " + m_responceCode 
                    + "\n" + "Content-Type: " + m_contentType + "\n" 
                    + "Content-Length: " + std::to_string(m_contentLength) + "\n\n"
                    + m_responceBody;

    write(m_clientFd, resultMessage.c_str(), resultMessage.size());
}
