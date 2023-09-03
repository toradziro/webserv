#include <CGI.hpp>
#include <FileFuncs.hpp>
#include <mutex>

const int readEnd = 0;
const int writeEnd = 1;
const int bufferSize = 8192;

std::mutex evrionsMutex;

CGI::CGI(const RequestConfig& requestConfig) {
    std::string fileExtention = getFileExtention(requestConfig.m_location);
    if(fileExtention == "" && requestConfig.m_allowedCGI->Has("binary")) {
        m_args.push_back(requestConfig.m_location);
    } else if(!requestConfig.m_allowedCGI->Has(fileExtention)) {
        // responce 500
        m_isError = true;
        checkError(true, "server is not allowed to execute this extention of file");
    } else {
        m_args.push_back(requestConfig.m_allowedCGI->Get(fileExtention));
        m_args.push_back(requestConfig.m_location);
    }
    m_cgiMode = m_args.size() == 1 ? CGIMode::CM_BINARY : CGIMode::CM_INTERPRETER;
    setEnviromentVariables(requestConfig);
}

void CGI::processCGI() {
    char *argForExec[3] = { NULL, NULL, NULL };
    if(m_cgiMode == CGIMode::CM_BINARY) {
        argForExec[0] = strdup(m_args[0].c_str());
    } else {
        argForExec[0] = strdup(m_args[0].c_str());
        argForExec[1] = strdup(m_args[1].c_str());
    }

    std::cout << "==" << argForExec[0] << "====" << argForExec[1] << "==" << std::endl;

    int readFd[2];
	int writeFd[2];
    char buffer[bufferSize];

    if(pipe(readFd) == -1) {
        setError("firstPipe wasn't created");
    }
    if(pipe(writeFd) == -1) {
        setError("secondPipe wasn't created");
    }
    // lock mutex to protect evrions be rewriten by other threads
    evrionsMutex.lock();
    for(auto& it : m_envs) {
        setenv(it.first.c_str(), it.second.c_str(), 1);
    }
    pid_t child = fork();
    if(child == 0) {
        dup2(writeFd[readEnd], STDIN_FILENO);
		dup2(readFd[writeEnd], STDOUT_FILENO);
		
        close(writeFd[writeEnd]);
		close(readFd[readEnd]);
        
        if(execve(argForExec[0], argForExec, environ) == -1) {
            setError("execve error");
        }
        exit(0);
    } else if(child > 0) {
        close(writeFd[readEnd]);
		close(readFd[writeEnd]);

        if(write(writeFd[writeEnd], m_msgForChild.c_str(), m_msgForChild.size()) == -1) {
            setError("write to CGI error");
        }
        close(writeFd[writeEnd]);
        // here we expect child process to be started
        evrionsMutex.unlock();
        
        waitpid(child, NULL, 0);
        memset(buffer, 0, bufferSize);
        while(read(readFd[readEnd], buffer, bufferSize) > 0) {
            m_outputBuffer += std::string(buffer);
        }
        close(readFd[readEnd]);
        std::cout << m_outputBuffer << std::endl;
    } else {
        evrionsMutex.unlock();
        setError("fork didn't work");
    }
    free(argForExec[0]);
    if(m_cgiMode == CGIMode::CM_INTERPRETER) {
        free(argForExec[1]);
    }
}

static std::string requestTypeToString(RequestType requestType) {
    switch (requestType)
    {
    case RT_GET:
        return "GET";
        break;
    case RT_POST:
        return "POST";
        break;
    case RT_PUT:
        return "PUT";
        break;
    
    default:
        break;
    }
    return "";
}

void CGI::setEnviromentVariables(const RequestConfig& requestConfig) {
    m_envs["QUERY_STRING"] = requestConfig.m_queryString;
    m_envs["CONTENT_LENGTH"] = requestConfig.entityHeaderTable.Get("Content-Length");
    m_envs["CONTENT_TYPE"] = requestConfig.entityHeaderTable.Get("Content-Type");
    m_envs["GATEWAY_INTERFACE"] = "CGI/1.1";
    m_envs["PATH_INFO"] = requestConfig.m_location;
    m_envs["PATH_TRANSLATED"] = requestConfig.m_location;
    m_envs["REMOTE_HOST"] = requestConfig.requestHeaderTable.Get("User-Agent");
    m_envs["REQUEST_METHOD"] = requestTypeToString(requestConfig.m_reqType);
    m_envs["SERVER_PROTOCOL"] = "HTTP/1.1";
    m_envs["SERVER_NAME"] = requestConfig.m_serverName;
    m_envs["SERVER_ROOT"] = requestConfig.m_serverRoot;
}

void CGI::setError(const std::string& discription) {
    checkError(true, discription);
    m_isError = true;
}