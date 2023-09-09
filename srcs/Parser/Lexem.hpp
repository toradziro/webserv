#pragma once

#include <common_inc.h>
#include <Errors.hpp>
#include <ConfigError.hpp>
#include <Parser.hpp>
#include <LexemInterface.hpp>

class LocationLexem : public InterfaceLexem {
public:
    LocationLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~LocationLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    // key - label, value - rootPath
    std::pair<std::string, std::string> m_location;

    ConfigErrors* m_errors;
};

class ListenLexem : public InterfaceLexem {
public:
    ListenLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~ListenLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string m_ipAddress;
    uint16_t    m_port = 0;

    ConfigErrors* m_errors;
};

class ServerNameLexem : public InterfaceLexem {
public:
    ServerNameLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~ServerNameLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string m_serverName;

    ConfigErrors* m_errors;
};

class ServerRootLexem : public InterfaceLexem {
public:
    ServerRootLexem(ConfigErrors* errors) : m_errors(errors) {}
    ~ServerRootLexem() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string m_parsedPath;

    ConfigErrors* m_errors;
};

class CgiAllowed : public InterfaceLexem {
public:
    CgiAllowed(ConfigErrors* errors) : m_errors(errors) {}
    ~CgiAllowed() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string   m_allowedExtention;
    std::string   m_extentionExecuter;

    ConfigErrors* m_errors;
};

class CgiDirectory : public InterfaceLexem {
public:
    CgiDirectory(ConfigErrors* errors) : m_errors(errors) {}
    ~CgiDirectory() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    std::string   m_cgiDirectory;

    ConfigErrors* m_errors;
};

//working_threads
class WorkingThreads : public InterfaceLexem {
public:
    WorkingThreads(ConfigErrors* errors) : m_errors(errors) {}
    ~WorkingThreads() {}
    // Interface realization
    void parseLexem(const std::vector<token> tokens, size_t& currentIndex) override;
    void addToServer(Server* serv) override;

private:
    int           m_threadsCount = -1;

    ConfigErrors* m_errors;
};

InterfaceLexem* createLexemByToken(const token& _token, ConfigErrors* errors);
