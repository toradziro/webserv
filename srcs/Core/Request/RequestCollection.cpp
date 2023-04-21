#include <RequestCollection.hpp>

static char* readNextChunk(int clientFd) {
    char* clientMessage = (char*)calloc(MSG_LEN, sizeof(char));
    checkError(clientMessage == NULL, "didn't allocated space for client");
    int messageLength = recv(clientFd, clientMessage, MSG_LEN, 0);
    // checkError(messageLength == -1, "tried to receive msg from new client");
    if(messageLength == -1) {
        return nullptr;
    }
    return clientMessage;
}

void RequestCollection::processMessage(int clientFd) {
    RequestInterface* request;
    if(getRequest(clientFd, &request)) {
        keepHandle(clientFd, request);
    } else {
        newConnection(clientFd);
    }
}

void RequestCollection::newConnection(int clientFd) {
    char* clientMessage = readNextChunk(clientFd);
    if(clientMessage == nullptr || strlen(clientMessage) == 0) {
        return;
    }
    RequestInterface* request = m_fabric.create(clientMessage, m_locations, m_contentTypes, clientFd, m_serverRoot);
    int reqVal = request->handleRequest();
    if(reqVal == -1) {
        m_unfinishedRequests.push_back(request);
    } else {
        delete request;
    }
}

void RequestCollection::keepHandle(int clientFd, RequestInterface* currRec) {
    char* nextChunk = readNextChunk(clientFd);
    if(nextChunk == nullptr) {
        return;
    }
    nextChunk = strcat(currRec->getBuffer(), nextChunk);
    currRec->setBuffer(nextChunk);
    int reqVal = currRec->handleRequest();
    if(reqVal == -1) {
        return;
    } else {
        deleteFromCollection(clientFd);
        delete currRec;
    }
}

bool RequestCollection::getRequest(int clientFd, RequestInterface** request) {
    *request = nullptr;
    for(size_t i = 0; i < m_unfinishedRequests.size(); ++i) {
        if(m_unfinishedRequests[i]->getClientFd() == clientFd) {
           *request = m_unfinishedRequests[i];
           return true;
        }
    }
    return false;
}

void RequestCollection::deleteFromCollection(int clientFd) {
    size_t i = 0;
    for(; i < m_unfinishedRequests.size(); ++i) {
        if(m_unfinishedRequests[i]->getClientFd() == clientFd) {
            break;
        }
    }
    m_unfinishedRequests.erase(m_unfinishedRequests.begin() + i);
}
