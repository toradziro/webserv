#include <common_inc.h>
#include <Parser.hpp>
#include <Server.hpp>
#include <ServerCreator.hpp>
#include <Log.hpp>

static void usage(char* progName) {
    std::cout << "Usage: " << progName << " <config_name.conf>" << std::endl;
}

int main(int argc, char** argv) {
    // parse config
    if(argc != 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    try {
        Config* serverConfig = Parser::parseConfig(argv[1]);
        serverConfig->validateConfig();
        ServerCreator serverCreator(serverConfig);
        Server* server = serverCreator.create();
        server->checkServerInstance();
        delete serverConfig;

#ifdef _DEBUG
        server->printServer();
#endif
        server->prepareForStart();
        server->start();
        delete server;
    } catch(CException* e) {
        LogMessage(e->getDescription());
        delete e;
    }

    exit(EXIT_SUCCESS);
}