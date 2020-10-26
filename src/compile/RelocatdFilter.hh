//
// Created by dos DRTT on 10/24/20.
//

#include <string>
#include <map>
#include "standard.hh"

#ifndef MACHINE_RELOCATDFILTER_HH
#define MACHINE_RELOCATDFILTER_HH


class RelocatedFilter {
public:
    compile_register reg_replace = {{"eax", 0x1},
                                    {"ebx", 0x2},
                                    {"ecx", 0x3},
                                    {"edx", 0x4},
                                    {"esi", 0x5},
                                    {"edi", 0x6}};


    using compile_code = std::map<std::string, int>;


public:
    virtual bool match(char v) = 0;

    virtual decode_result relocate(std::string, address_map *mp) = 0;
};

class InstrumentFilter {
    compile_code instrument_table = {{"push", 0xb},
                                     {"pop", 0x1},
                                     {"call", 0x2},
                                     {"ret", 0x3},
                                     {"mov", 0x4},
                                     {"echo", 0x77},
                                     {"add_process", 0x5},
                                     {"jmp", 0x6},
                                     {"je", 0x7},
                                     {"jne", 0x8},
                                     {"exit", 0x9},
                                     {"cmp", 0xa},
    };
public:
    bool match(char v) {
        return true;
    }

    decode_result relocate(std::string reg) {
        return decode_result(instrument_table[reg], false);
    }

};


class RegisterFilter : public RelocatedFilter {
public:

    bool match(char v) override {
        return v == '$';
    }

    decode_result relocate(std::string reg, address_map *mp) override {
        return decode_result(reg_replace[reg.substr(1, reg.length())], false);
    }
};

class AddressFilter : public RelocatedFilter {
public:

    bool match(char v) override {
        return v == '#';
    }

    decode_result relocate(std::string addr, address_map *mp) override {
        return decode_result((*mp)[addr + ":"], true);
    }
};


class NumberFilter : public RelocatedFilter {
public:

    bool match(char v) override {
        return v == '%';
    }

    decode_result relocate(std::string num, address_map *mp) override {
        return decode_result(std::stoi(num), true);
    }
};

class Product {
    std::list<RelocatedFilter *> filter_chain;
public:
    Product(const std::initializer_list<RelocatedFilter *> &l) {
        for (auto b = l.begin(); b != l.end(); b++) {
            filter_chain.push_back(*b);
        }
    }

    decode_result do_chain(std::string &opval, address_map *mp) {
        for (auto f : filter_chain) {
            if (f->match(opval[0])) {
                return f->relocate(opval.substr(1, opval.length()), mp);
            }
        }
        throw -2;
    }
};

#endif //MACHINE_RELOCATDFILTER_HH