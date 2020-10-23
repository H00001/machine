//
// Created by dos DRTT on 6/14/20.
//

#include <string>
#include <list>

#ifndef MACHINE_UTIL_HH
#define MACHINE_UTIL_HH

#endif //MACHINE_UTIL_HH





class strings {
private:
    static std::string &ClearHeadTailSpace(std::string &str) {
        if (str.empty()) {
            return str;
        }
        str.erase(0, str.find_first_not_of(' '));
        str.erase(str.find_last_not_of(' ') + 1);
        return str;
    }

public:


    static std::string &trim(std::string &str) {
        str.erase(str.find_last_not_of(' ') + 1);
        str.erase(0, str.find_first_not_of(' '));
        return str;
    }

    static std::string *spilt(std::string &s, const std::string &s1) {
        s = ClearHeadTailSpace(s);
        auto *r = new std::string[2];
        int index = s.find(s1);
        r[0] = s.substr(0, index);
        r[1] = (index < 0) ? "" : s.substr(index + 1, s.length());
        return r;
    }



private:

};