#include <Responce.hpp>

Responce::Responce(int clientFd) : m_clientFd(clientFd) {} 

void Responce::sendResponce() {
    const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    write(m_clientFd , hello , strlen(hello));
}
