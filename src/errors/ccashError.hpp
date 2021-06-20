#pragma once
#include <string>

namespace error {
    using namespace std;
    static int replacePart(string &s, string from, string to) {
        size_t position = s.find(from);
        int32_t changed = 0;

        while(position != string::npos) {
            s.replace(position, from.size(), to);
            changed++;
            position = s.find(from, position + to.size());
        }
        return changed;
    }
}