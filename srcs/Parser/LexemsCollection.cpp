#include <LexemsCollection.hpp>

LexemsCollection::~LexemsCollection() {
    for(size_t i = 0; i < lexems.size(); ++i) {
        delete lexems[i];
    }
}

void LexemsCollection::parseLexem(const std::vector<token> /*tokens*/, size_t& /*currentIndex*/) {
    checkError(true, "not allowed to call");
}

void LexemsCollection::addToServer(Server* serv) {
    for(size_t i = 0; i < lexems.size(); ++i) {
        lexems[i]->addToServer(serv);
    }
}

void LexemsCollection::addLexem(Lexem* lexem) {
    lexems.push_back(lexem);
}
