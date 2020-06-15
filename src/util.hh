//
// Created by dos DRTT on 6/14/20.
//

#include <string>
#include <list>

#ifndef MACHINE_UTIL_HH
#define MACHINE_UTIL_HH

#endif //MACHINE_UTIL_HH

enum operType {
    num, reg, str
};

struct Operator {
    std::string operReg;
    std::string operStr;
    int operval{};
    operType type;
};

class strings {

public:
    static std::string *spilt(std::string &s, std::string s1) {
        auto *r = new std::string[2];
        int index = s.find(s1);
        r[0] = s.substr(0, index);
        r[1] = (index < 0) ? "" : s.substr(index + 1, s.length());
        return r;
    }

    static std::list<Operator *> *Analyze(std::string &s) {
        auto *list = new std::list<Operator *>;
        int opIndex = s.find(',');
        if (opIndex < 0) {
            list->push_back(operAna(s));
        } else {
            auto op = spilt(s, ",");
            auto l = op[0];
            auto r = op[1];
            list->push_back(operAna(l));
            list->push_back(operAna(r));
        }
        return list;
    }

private:
    static Operator *operAna(std::string &o) {
        auto *op = new Operator;
        char flag = o.c_str()[0];
        if (flag == '%') {
            op->type = num;
            op->operval = std::stoi(o.substr(1, o.length()));
        } else if (flag == '$') {
            op->type = reg;
            op->operReg = (o.substr(1, o.length()));
        } else {
            op->type = str;
            op->operStr = o;
        }
        return op;
    }
};