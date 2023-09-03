#include <CommonResponseSender.hpp>
#include <FileFuncs.hpp>

CommonResponseSender::CommonResponseSender(int clientFd, int fileFd, 
                                    std::string responseCode,
                                    std::string contentType,
                                    std::string filename) :
                                m_responseCode(std::move(responseCode)),
                                m_contentType(std::move(contentType)),
                                m_filename(std::move(filename)),
                                m_ResponseFSM(PreparationType::PT_File),
                                m_clientFd(clientFd),
                                m_fileFd(fileFd)
{}

CommonResponseSender::CommonResponseSender(int clientFd, std::string responseBody, 
                                        std::string responseCode) :
                                    m_responseBody(std::move(responseBody)),
                                    m_responseCode(std::move(responseCode)),
                                    m_ResponseFSM(PreparationType::PT_Buffer),
                                    m_clientFd(clientFd)
{}

void CommonResponseSender::sendResponse() {
    int bodySize = 0;
    if(m_ResponseFSM == PreparationType::PT_File) {
        bodySize = getFileSize(m_filename);
        std::string greetingMessage = httpType + " " + m_responseCode + "\n" 
                + "Content-Type: " + m_contentType + "\n" 
                + "Content-Length: " + std::to_string(bodySize) + "\n\n";
        send(m_clientFd, greetingMessage.c_str(), greetingMessage.size(), 0);
    } else if(m_ResponseFSM == PreparationType::PT_Buffer) {
        // Sending header only, because all of other parts alredy in body
        std::string greetingMessage = httpType + " " + m_responseCode + "\n";
        send(m_clientFd, greetingMessage.c_str(), greetingMessage.size(), 0);
        bodySize = m_responseBody.size();
    }

    int wasSent = 0;
    while(wasSent < bodySize) {
        int currSent = 0;
        currSent = getNextChank();
        send(m_clientFd, m_buffer, bufferSize, 0);
        wasSent += currSent;
    }
    if(m_fileFd != -1) {
        close(m_fileFd);
    }
    close(m_clientFd);
    std::cout << "succesfully sent data\n" << std::endl;
}

int CommonResponseSender::getNextChank() {
    memset(m_buffer, 0, bufferSize);
    int res = 0;
    if(m_ResponseFSM == PreparationType::PT_Buffer) {
        while(res < bufferSize && m_currentBufferIt < m_responseBody.size()) {
            m_buffer[res] = m_responseBody[m_currentBufferIt];
            ++m_currentBufferIt;
            ++res;
        }
    } else {
        res = read(m_fileFd, m_buffer, bufferSize);
    }
    return res;
}
