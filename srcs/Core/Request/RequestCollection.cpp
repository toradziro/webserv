#include <RequestCollection.hpp>

static char* readNextChunk(int clientFd) {
    char* clientMessage = (char*)calloc(MSG_LEN, sizeof(char));
    int messageLength = recv(clientFd, clientMessage, MSG_LEN, 0);
    checkError(messageLength == -1, "tried to receive msg from new client");
    return clientMessage;
}

void RequestCollection::processMessage(int clientFd) {
    if(hasUnfinishedRequest(clientFd)) {
        keepHandle(clientFd);
    } else {
        newConnection(clientFd);
    }
}

bool RequestCollection::hasUnfinishedRequest(int clientFd) {
    for(size_t i = 0; i < m_unfinishedRequests.size(); ++i) {
        if(m_unfinishedRequests[i]->getClientFd() == clientFd) {
            return true;
        }
    }
    return false;
}

void RequestCollection::newConnection(int clientFd) {
    char* clientMessage = readNextChunk(clientFd);
    RequestInterface* request = requestFabric(clientMessage, m_locations, clientFd);
    int reqVal = request->handleRequest();
    if(reqVal == -1) {
        m_unfinishedRequests.push_back(request);
    } else {
        delete request;
    }
}

void RequestCollection::keepHandle(int clientFd) {
    char* nextChunk = readNextChunk(clientFd);
    RequestInterface* currRec = getRequest(clientFd);
    nextChunk = strcat(currRec->getBuffer(), nextChunk);
    // add check
    // fix code double
    currRec->setBuffer(nextChunk);
    int reqVal = currRec->handleRequest();
    if(reqVal == -1) {
        m_unfinishedRequests.push_back(currRec);
    } else {
        deleteFromCollection(clientFd);
        delete currRec;
    }
}

RequestInterface* RequestCollection::getRequest(int clientFd) {
    for(size_t i = 0; i < m_unfinishedRequests.size(); ++i) {
        if(m_unfinishedRequests[i]->getClientFd() == clientFd) {
           return m_unfinishedRequests[i];
        }
    }
    return nullptr;
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
