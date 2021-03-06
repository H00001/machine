//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_STRINGS_HH
#define MACHINE_STRINGS_HH


//
// Created by dos DRTT on 6/14/20.
//

#include <string>
#include <list>
#include <regex>
#include <vector>

#ifndef MACHINE_UTIL_HH
#define MACHINE_UTIL_HH


const std::regex ws_space("\\s+");
const std::regex ws_reg(",");


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

    static std::vector<std::string> spilt(std::string &s) {
        if (s.find(' ') > 0) {
            std::vector<std::string> v(std::sregex_token_iterator(s.begin(), s.end(), ws_space, -1),
                                       std::sregex_token_iterator());
            return v;
        } else {
            return std::vector<std::string>({s, ""});
        }
    }

    static std::vector<std::string> spilt_reg(std::string &s) {
        std::vector<std::string> v(std::sregex_token_iterator(s.begin(), s.end(), ws_reg, -1),
                                   std::sregex_token_iterator());
        return v;
    }


private:

};

#endif //MACHINE_UTIL_HH


#endif //MACHINE_STRINGS_HH
