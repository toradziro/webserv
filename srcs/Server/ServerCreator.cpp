#include <ServerCreator.hpp>

ServerCreator::ServerCreator(Config* config) : m_serverConfig(config) {}

Server* ServerCreator::create() {
    Server* server = new Server();
    m_serverConfig->m_lexems->addToServer(server);
    server->setContentTypeCollection(m_serverConfig->m_contentTypes);
    return server;
}
