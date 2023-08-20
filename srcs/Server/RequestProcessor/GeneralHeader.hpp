#pragma once

#include <string>
#include <unordered_map>

/*
general-header = Cache-Control            ; Section 14.9
               | Connection               ; Section 14.10
               | Date                     ; Section 14.18
               | Pragma                   ; Section 14.32
               | Trailer                  ; Section 14.40
               | Transfer-Encoding        ; Section 14.41
               | Upgrade                  ; Section 14.42
               | Via                      ; Section 14.45
               | Warning                  ; Section 14.46
*/

class GeneralHeader {
public:
    GeneralHeader() {
        table["Cache-Control:"] = "";
        table["Connection:"] = "";
        table["Date:"] = "";
        table["Pragma:"] = "";
        table["Trailer:"] = "";
        table["Transfer-Encoding:"] = "";
        table["Upgrade:"] = "";
        table["Via:"] = "";
        table["Warning:"] = "";
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