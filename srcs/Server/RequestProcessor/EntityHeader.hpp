#pragma once

#include <string>
#include <unordered_map>

/*
entity-header  = Allow                    ; Section 14.7
               | Content-Encoding         ; Section 14.11
               | Content-Language         ; Section 14.12
               | Content-Length           ; Section 14.13
               | Content-Location         ; Section 14.14
               | Content-MD5              ; Section 14.15
               | Content-Range            ; Section 14.16
               | Content-Type             ; Section 14.17
               | Expires                  ; Section 14.21
               | Last-Modified            ; Section 14.29
               | extension-header
extension-header = message-header
*/

class EntityHeader {
public:
    EntityHeader() {
        table["Allow:"] = "";
        table["Content-Encoding:"] = "";
        table["Content-Language:"] = "";
        table["Content-Length:"] = "";
        table["Content-Location:"] = "";
        table["Content-MD5:"] = "";
        table["Content-Range:"] = "";
        table["Content-Type:"] = "";
        table["Expires:"] = "";
        table["Last-Modified:"] = "";
    }

    bool Has(const std::string& string) const {
        return table.count(string) == 1;
    }

    std::string Get(const std::string& key) {
        return table[key];
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