#pragma once

#include <common_inc.h>
#include <RequestConfig.hpp>

enum class CGIMode {
    CM_BINARY,
    CM_INTERPRETER,
};

class CGI {
public:
    explicit CGI(const RequestConfig& requestConfig);
    
    ~CGI() {}

    void processCGI();
    
    std::string getBuffer() {
        return std::move(m_outputBuffer);
    }

    [[nodiscard]]bool isError() {
        return m_isError;
    }

private:
    CGI() = delete;
    CGI(CGI&) = delete;
    CGI operator=(CGI&) = delete;
    void setError(const std::string& discription);
    void setEnviromentVariables(const RequestConfig& requestConfig);

    std::unordered_map<std::string, std::string> m_envs;
    std::vector<std::string>                     m_args;
    std::string                                  m_outputBuffer;
    std::string                                  m_msgForChild;
    CGIMode                                      m_cgiMode;
    bool                                         m_isError = false;
};
