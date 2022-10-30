#pragma once

#include <Config.hpp>
#include <Server.hpp>

class ServerCreator {
public:
    explicit ServerCreator(Config* config);
    Server* create();
private:
    Config* m_serverConfig;
};
