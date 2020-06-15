//
// Created by dos DRTT on 6/14/20.
//

#include <string>

#ifndef MACHINE_UTIL_HH
#define MACHINE_UTIL_HH

#endif //MACHINE_UTIL_HH

class strings {
public:
    static std::string *spilt(std::string &s, std::string s1) {
        auto *r = new std::string[2];
        int index = s.find(s1);
        r[0] = s.substr(0, index);
        r[1] = (index < 0) ? "" : s.substr(index + 1, s.length());
        return r;
    }
};