#include <common_inc.h>
#include <FileFuncs.hpp>
#include <GetRequestHandler.hpp>
#include <CommonResponseSender.hpp>
#include <CGI.hpp>

const std::string autoindexHeader = "<!DOCTYPE html>\n<html>\n<body>\n\t<h1>AUTOGENERATED INDEX HTML</h1>\n";
const std::string autoindexEnd = "</body>\n</html>\n";
const std::string indexFileNameHtml = "/index.html";

GetRequestHandler::GetRequestHandler(RequestConfig requestConfig) : 
                m_requestConfig(std::move(requestConfig))
{
}

void GetRequestHandler::prepareResponce() {
    if(validateLocation(m_requestConfig.m_serverRoot, m_requestConfig.m_location) && isFileExists(m_requestConfig.m_location)) {
    } else {
        m_contentType = "text/html";
        m_responseCode = Error404;
        m_requestConfig.m_location = page404;
    }

    FileType fType = fileType(m_requestConfig.m_location);

    if(validateLocation(m_requestConfig.m_CGILocation, m_requestConfig.m_location) && fType != FT_DIRECTORY) {
        m_isCGIScenario = true;
    }
    if(m_isCGIScenario) {
        CGI CGIProcessos(m_requestConfig);
        CGIProcessos.processCGI();
        if(CGIProcessos.isError()) {
            m_responseCode = Error500;
        } else {
            m_responseCode = Ok200;
            m_CGIResponceBody = std::move(CGIProcessos.getBuffer());
        }
        return;
    }

    m_responseCode = Ok200;
    m_contentType = m_requestConfig.m_contentTypes->getContentTypeByExtention(getFileExtention(m_requestConfig.m_location));
    switch(fType)
    {
        case FT_FILE:
            // just open file for ahead reading
            m_fileFd = open(m_requestConfig.m_location.c_str(), O_RDONLY);
            checkError(m_fileFd == -1, "can't open file");
            break;

        case FT_DIRECTORY:
            // need to check index.html, if no - generate it
            fillIndex();
            break;
        case FT_NONE:
        default:
            // TODO: fix to log
            checkError(true, "unknown file type");
            break;
    }
}

void GetRequestHandler::sendResponse() {
    if(m_isAutoindex) {
        CommonResponseSender responseSender(m_requestConfig.m_clientFd, std::move(m_generatedAutoindex), std::move(m_responseCode));
        responseSender.sendResponse();
    } else if(m_isCGIScenario) {
        CommonResponseSender responseSender(m_requestConfig.m_clientFd, std::move(m_CGIResponceBody), std::move(m_responseCode));
        responseSender.sendResponse();
    } else {
        CommonResponseSender responseSender(m_requestConfig.m_clientFd, m_fileFd,
                                    std::move(m_responseCode), std::move(m_contentType), 
                                    std::move(m_requestConfig.m_location));
        responseSender.sendResponse();
    }
}

void GetRequestHandler::fillIndex() {
    if(dirContainsFile(m_requestConfig.m_location, "index.html")) {
        if(m_requestConfig.m_location[m_requestConfig.m_location.size() - 1] != '/') {
            m_requestConfig.m_location += '/';
        }
        m_requestConfig.m_location += "index.html";
        m_fileFd = open(m_requestConfig.m_location.c_str(), O_RDONLY);
        checkError(m_fileFd == -1, "can't open file");
        m_bodySize = getFileSize(m_requestConfig.m_location);
    } else {
        // generating autoindex
        m_isAutoindex = true;
        m_generatedAutoindex += httpType + " " + m_responseCode + "\n" 
                + "Content-Type: " + m_contentType + "\n\n";
        m_generatedAutoindex += autoindexHeader;
        std::vector<std::filesystem::path> filesList = listDirContent(m_requestConfig.m_location);
        std::sort(filesList.begin(), filesList.end());
        for(size_t i = 0; i < filesList.size(); ++i) {
            std::string body = ""; 
            body += "\t<a href=\"http://"; 
            body += m_requestConfig.requestHeaderTable.Get("Host:");
            body += filesList[i].string();
            body += "\">" + filesList[i].filename().string() + "</a><br>\n";
            m_generatedAutoindex += body;
        }
        m_generatedAutoindex += autoindexEnd;
        m_bodySize = m_generatedAutoindex.size();
    }
}
