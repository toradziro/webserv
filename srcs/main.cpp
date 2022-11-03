#include <common_inc.h>
#include <Parser.hpp>
#include <Server.hpp>
#include <ServerCreator.hpp>

volatile std::sig_atomic_t gSignalStatus = 0;

static void usage(char* progName) {
    std::cout << "Usage: " << progName << " <config_name.conf>" << std::endl;
}

extern "C" void signalHandler(int signum) {
   std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
   gSignalStatus = signum;  
}

int main(int argc, char** argv) {
    // parse config
    if(argc != 2) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Register signal to handle close properly
    std::signal(SIGQUIT, signalHandler);

    try {
        Config* serverConfig = Parser::parseConfig(argv[1]);
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
    } catch (CException* e) {
        std::cout << e->getDescription() << std::endl;
        delete e;
    }

    exit(EXIT_SUCCESS);
}