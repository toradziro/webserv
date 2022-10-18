#pragma once

#include <Lexem.hpp>

class LexemsCollection : public Lexem::InterfaceLexem {
public:
    LexemsCollection() {}
    // Need to free all interfaces after fill
    ~LexemsCollection();

    // Interface realization
    void parseLexem(const std::vector<token> /*tokens*/, size_t& /*currentIndex*/);
    // Add all lexems to a server
    void addToServer(Server* serv);

    void addLexem(InterfaceLexem* lexem);
    // move semantic allowance
    LexemsCollection(LexemsCollection&& other) {
        m_lexems = std::move(other.m_lexems);
    }
private:
    std::vector<InterfaceLexem*> m_lexems;
};