#include <PostRequestHandler.hpp>
#include <FileFuncs.hpp>
#include <CommonResponseSender.hpp>
#include <ErrorSender.hpp>
#include <CGI.hpp>

PostRequestHandler::PostRequestHandler(RequestConfig requestConfig) : 
                m_requestConfig(std::move(requestConfig))
{ }

void PostRequestHandler::prepareResponce() {
    if(!validateLocation(m_requestConfig.m_serverRoot, m_requestConfig.m_location) || !isFileExists(m_requestConfig.m_location)) {
        sendError(ErrorCode::EC_404, m_requestConfig.m_clientFd);
        m_isError = true;
        return;
    } else {
        m_responseCode = Ok200;
    }
    if(!validateLocation(m_requestConfig.m_CGILocation, m_requestConfig.m_location)) {
        sendError(ErrorCode::EC_403, m_requestConfig.m_clientFd);
        m_isError = true;
        return;
    }
    // m_requestConfig.entityHeaderTable.debugPrint();
    CGI cgiProcessor(m_requestConfig);
    cgiProcessor.processCGI();
    if(cgiProcessor.isError()) {
        sendError(ErrorCode::EC_500, m_requestConfig.m_clientFd);
        m_isError = true;
        return;
    }
    m_responseBody = std::move(cgiProcessor.getBuffer());
}

void PostRequestHandler::sendResponse() {
    if(m_isError) {
        return;
    }
    CommonResponseSender responseSender(m_requestConfig.m_clientFd, std::move(m_responseBody), m_responseCode);
    responseSender.sendResponse();
}

// void PostRequestHandler::prepareEnvs() {
//     // setenv(const char *name, const char *value, int overwrite);
//     setenv("GATEWAY_INTERFACE", "CGI/0.0", 1);
//     setenv("REQUESTED_LOCATION", m_requestConfig.m_location.c_str(), 1);
//     setenv("SERVER_ROOT", m_requestConfig.m_serverRoot.c_str(), 1);
//     setenv("SERVER_PROTOCOL", httpType.c_str(), 1);
//     setenv("REQUEST_METHOD", "POST", 1);
// }
