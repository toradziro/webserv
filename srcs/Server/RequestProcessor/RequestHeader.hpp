#pragma once

#include <string>
#include <unordered_map>

/*
request-header = Accept                   ; Section 14.1
                | Accept-Charset           ; Section 14.2
                | Accept-Encoding          ; Section 14.3
                | Accept-Language          ; Section 14.4
                | Authorization            ; Section 14.8
                | Expect                   ; Section 14.20
                | From                     ; Section 14.22
                | Host                     ; Section 14.23
                | If-Match                 ; Section 14.24
                | If-Modified-Since        ; Section 14.25
                | If-None-Match            ; Section 14.26
                | If-Range                 ; Section 14.27
                | If-Unmodified-Since      ; Section 14.28
                | Max-Forwards             ; Section 14.31
                | Proxy-Authorization      ; Section 14.34
                | Range                    ; Section 14.35
                | Referer                  ; Section 14.36
                | TE                       ; Section 14.39
                | User-Agent               ; Section 14.43
*/

class RequestHeader {
public:
    RequestHeader() {
        table["Accept:"] = "";
        table["Accept-Charset:"] = "";
        table["Accept-Encoding:"] = "";
        table["Accept-Language:"] = "";
        table["Authorization:"] = "";
        table["Expect:"] = "";
        table["From:"] = "";
        table["Host:"] = "";
        table["If-Match:"] = "";
        table["If-Modified-Since:"] = "";
        table["If-None-Match:"] = "";
        table["If-Range:"] = "";
        table["If-Unmodified-Since:"] = "";
        table["Max-Forwards:"] = "";
        table["Proxy-Authorization:"] = "";
        table["Range:"] = "";
        table["Referer:"] = "";
        table["TE:"] = "";
        table["User-Agent:"] = "";
    }

    bool Has(const std::string& string) const {
        return table.count(string) == 1;
    }

    void AddValue(const std::string& key, const std::string& value) {
        table[key] = value;
    }

    void debugPrint() {
        for(auto it : table) {
            std::cout << it.first << " ----- " << it.second << std::endl;
        }
    }
    
private:
    std::unordered_map<std::string, std::string> table;     
};