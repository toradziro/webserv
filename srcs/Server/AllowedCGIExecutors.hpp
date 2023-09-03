#pragma once

#include <common_inc.h>

struct AllowedCGIExecutors {
    void Add(const std::string& key, const std::string& value) {
        executors[key] = value;
    }

    std::string Get(const std::string& key) {
        return executors[key];
    }

    bool Has(const std::string& key) {
        return executors.count(key) == 1;
    }

private:
    std::unordered_map<std::string, std::string> executors;
};
