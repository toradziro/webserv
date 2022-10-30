#include <Response.hpp>
#include <FileFuncs.hpp>

// "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!"
// 404 Not Found
// 200 OK
// text/html

std::string httpType = "HTTP/1.1";

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
    unsigned char* fileMapping = (unsigned char*)mmap(NULL,
                            m_fileSize,
                            PROT_READ,
                            MAP_PRIVATE,
                            m_fileFd, 0);

    checkError(fileMapping == NULL, "Response mapping failed");

    std::string resultMessage = "";
    resultMessage += httpType + " " + m_ResponseCode + "\n" 
                    + "Content-Type: " + m_contentType + "\n" 
                    + "Content-Length: " + std::to_string(m_fileSize) + "\n\n";
    send(m_clientFd, resultMessage.c_str(), resultMessage.size(), 0);
    send(m_clientFd, fileMapping, m_fileSize + 1, 0);
    std::cout << "succesfully sent data" << std::endl;

    checkError(munmap((void*)fileMapping, m_fileSize) == -1, "Response unmapping failed");
}
