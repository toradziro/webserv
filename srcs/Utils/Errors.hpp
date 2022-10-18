#pragma once

#include <common_inc.h>

// made by tutorial http://ders.stml.net/cpp/intmsg/intmsg.html

#define _FLINE_ CException::fileLine(__FILE__, __LINE__)
// Macroces for exception generation
#define checkError(val, descr) if((val) == true){throw newCException(_FLINE_, descr);}

#define checkConfigError(val, fileName, line) if(val == false) \
            {throw newCException(CException::fileLine(fileName, line), "not valid syntax");}

class CException {
public:
    struct fileLine {
        fileLine(const char* f, int l) : fileName(f), line(l) {}
        const char* fileName;
        int line;
    };

    std::string getDescription() {
        return "CException [" + std::string(location.fileName) + \
                ":" + std::to_string(location.line) + "] " + description + \
                "\nerrno to string: " + std::string(strerror(errno));
    }

    friend CException* newCException(const fileLine& loc, const std::string& msg) {
        return new CException(loc, msg);
    }

protected:
    CException(const fileLine& loc, const std::string& msg) :  description(msg), location(loc) {}
private:
    std::string description;
    fileLine    location;
};
