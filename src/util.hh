//
// Created by dos DRTT on 6/14/20.
//

#include <string>
#include <list>

#ifndef MACHINE_UTIL_HH
#define MACHINE_UTIL_HH

#endif //MACHINE_UTIL_HH

using opFN = std::function<void(std::string)>;
using rHeap = std::map<std::string, unsigned long *>;
using operatorMap = std::map<std::string, std::function<void(std::string)>>;

enum operaType {
    num, reg, str
};

struct Operator {
    std::string operReg;
    std::string operStr;
    int operval{};
    operaType type;
};

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

    static std::string *spilt(std::string &s, const std::string& s1) {
        s = ClearHeadTailSpace(s);
        auto *r = new std::string[2];
        int index = s.find(s1);
        r[0] = s.substr(0, index);
        r[1] = (index < 0) ? "" : s.substr(index + 1, s.length());
        return r;
    }

    static std::list<Operator *> *Analyze(std::string &s, rHeap *h) {
        auto *list = new std::list<Operator *>;
        int opIndex = s.find(',');
        if (opIndex < 0) {
            list->push_back(operAna(s, h));
        } else {
            sp(s, h, list);
        }
        return list;
    }

    static void sp(std::string &s, rHeap *h, std::list<Operator *> *list) {
        auto op = spilt(s, ",");
        list->push_back(operAna(op[0], h));
        auto c = operAna(op[1], h);
        int k = c->operStr.find(',');
        if (k > 0) {
            sp(c->operStr, h, list);
        } else {
            list->push_back(c);
        }

    }

private:
    static Operator *operAna(std::string &o, rHeap *h) {
        auto *op = new Operator;
        char flag = o.c_str()[0];
        if (flag == '%') {
            op->type = num;
            op->operval = std::stoi(o.substr(1, o.length()));
        } else if (flag == '$') {
            op->type = reg;
            op->operReg = (o.substr(1, o.length()));
            op->operval = *(*h)[op->operReg];
        } else {
            op->type = str;
        }
        op->operStr = o;

        return op;
    }
};