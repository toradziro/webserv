#include <common_inc.h>
#include <Parser.hpp>
#include <Server.hpp>

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
        Server* serv = Parser::parseConfig(argv[1]);
#ifdef _DEBUG
        serv->printServer();
#endif
        serv->prepareForStart();
        serv->start();
        delete serv;
    } catch (CException* e) {
        std::cout << e->getDescription() << std::endl;
        delete e;
    }

    exit(EXIT_SUCCESS);
}