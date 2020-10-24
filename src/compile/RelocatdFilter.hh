//
// Created by dos DRTT on 10/24/20.
//

#include <string>
#include <map>

#ifndef MACHINE_RELOCATDFILTER_HH
#define MACHINE_RELOCATDFILTER_HH

#endif //MACHINE_RELOCATDFILTER_HH

enum type {
    operation, reg, addr, empty
};

class RelocatedFilter {
public:
    using compile_register = std::map<std::string, int>;
    compile_register reg_replace = {{"eax", 0x1},
                                    {"ebx", 0x2},
                                    {"ecx", 0x3},
                                    {"edx", 0x4},
                                    {"esi", 0x5},
                                    {"edi", 0x6}};


    using compile_code = std::map<std::string, int>;

    compile_code instrument_table = {{"push",        0xb},
                                     {"pop",         0x1},
                                     {"call",        0x2},
                                     {"ret",         0x3},
                                     {"mov",         0x4},
                                     {"echo",        0x77},
                                     {"add_process", 0x5},
                                     {"jmp",         0x6},
                                     {"je",          0x7},
                                     {"jne",         0x8},
                                     {"exit",        0x9},
                                     {"cmp",         0xa},
    };
public:
    virtual type otype() = 0;

    virtual bool match(char v) = 0;

    virtual std::string relocate(std::string) = 0;
};

class InstrumentFilter : public RelocatedFilter {
public:
    type otype() override {
        return operation;
    }

    bool match(char v) override {
        return true;
    }

    std::string relocate(std::string reg) override {
        return std::to_string(instrument_table[reg]);
    }

};


class RegisterFilter : public RelocatedFilter {
public:
    type otype() override {
        return reg;
    }

    bool match(char v) override {
        return v == '$';
    }

    std::string relocate(std::string reg) override {
        return "$" + std::to_string(reg_replace[reg.substr(1, reg.length())]);
    }
};

class EmptyFilter : public RelocatedFilter {
public:
    type otype() override {
        return empty;
    }

    std::string relocate(std::string reg) override {
        return "0 %0";
    }

    bool match(char v) override {
        return true;
    }

};
