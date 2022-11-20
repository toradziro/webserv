#include <Response.hpp>
#include <FileFuncs.hpp>

std::string httpType = "HTTP/1.1";
#define BUFF_SIZE 4096

Response::Response(int clientFd,
            std::string ResponseCode,
            std::string contentType,
            int fileFd) :

            m_ResponseCode(ResponseCode),
            m_contentType(contentType),
            m_clientFd(clientFd),
            m_fileFd(fileFd)
{
    m_fileSize = getFileSize(m_fileFd);
}

void Response::sendResponse() {
    std::cout << "PREPARING RESPONCE" << std::endl;
    std::string resultMessage = "";
    resultMessage += httpType + " " + m_ResponseCode + "\n" 
                    + "Content-Type: " + m_contentType + "\n" 
                    + "Content-Length: " + std::to_string(m_fileSize) + "\n\n";
    send(m_clientFd, resultMessage.c_str(), resultMessage.size(), 0);
    unsigned char buff[BUFF_SIZE];
    int wasSent = 0;
    while(wasSent < m_fileSize) {
        int currSent = 0;
        currSent = read(m_fileFd, buff, BUFF_SIZE);
        send(m_clientFd, buff, BUFF_SIZE, 0);
        wasSent += currSent;
    }
    std::cout << "succesfully sent data\n" << std::endl;
}
