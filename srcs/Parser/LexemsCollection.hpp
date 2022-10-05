#pragma once

#include <Lexem.hpp>

class LexemsCollection : public Lexem::Lexem {
public:
    LexemsCollection() {}
    // Need to free all interfaces after fill
    ~LexemsCollection();

    // Interface realization
    void parseLexem(const std::vector<token> /*tokens*/, size_t& /*currentIndex*/);
    // Add all lexems to a server
    void addToServer(Server* serv);

    void addLexem(Lexem* lexem);
    // move semantic allowance
    LexemsCollection(LexemsCollection&& other) {
        lexems = std::move(other.lexems);
    }
private:
    std::vector<Lexem*> lexems;
};