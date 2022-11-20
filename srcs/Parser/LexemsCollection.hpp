#pragma once

#include <LexemInterface.hpp>

class LexemsCollection : public InterfaceLexem {
public:
    LexemsCollection() {}
    LexemsCollection(LexemsCollection& other) {
        m_lexems = other.getLexems();
    }
    // move semantic allowance
    LexemsCollection(LexemsCollection&& other) {
        m_lexems = std::move(other.m_lexems);
    }
    // Need to free all interfaces after fill
    ~LexemsCollection();

    // Interface realization
    void parseLexem(const std::vector<token> /*tokens*/, size_t& /*currentIndex*/);
    // Add all lexems to a server
    void addToServer(Server* serv);
    void addLexem(InterfaceLexem* lexem);
    std::vector<InterfaceLexem*> getLexems() {
        return m_lexems;
    }

private:
    std::vector<InterfaceLexem*> m_lexems;
};