#include <LexemsCollection.hpp>

LexemsCollection::~LexemsCollection() {
    for(size_t i = 0; i < m_lexems.size(); ++i) {
        delete m_lexems[i];
    }
}

void LexemsCollection::parseLexem(const std::vector<token> /*tokens*/, size_t& /*currentIndex*/) {
    checkError(true, "not allowed to call");
}

void LexemsCollection::addToServer(Server* serv) {
    for(size_t i = 0; i < m_lexems.size(); ++i) {
        m_lexems[i]->addToServer(serv);
    }
}

void LexemsCollection::addLexem(InterfaceLexem* lexem) {
    m_lexems.push_back(lexem);
}
