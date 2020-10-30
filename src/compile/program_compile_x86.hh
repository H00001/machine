//
// Created by dos DRTT on 10/24/20.
//
#ifndef MACHINE_PROGRAM_COMPILE_X86_HH
#define MACHINE_PROGRAM_COMPILE_X86_HH

#include <fstream>
#include <map>
#include <vector>
#include "program_compile.hh"
#include "RelocatedFilter.hh"
#include "../util/strings.hh"

class program_compile_x86 : public program_compile {

private:
    std::map<int, std::string> *debug_map;
    Product *p = new Product({new RegisterFilter(), new AddressFilter(), new NumberFilter()});
    InstrumentFilter *inf = new InstrumentFilter();
    word *b;
    std::string *da0;
    int len;
    int data_raw_len;
public:

    program_compile_x86() = default;

    ret compile_load(std::string file_name) override;

    void rewrite_to_file(std::string file_name) override;

    ~program_compile_x86() override;

private:
    unsigned int compile_code_segment(std::string *base, int length, word *lbuf, int &ddlen);

    unsigned int compile_data_segment(std::string *base, int length, word *, int &) {
        da0 = new std::string[length / 2];
        std::map<std::string, int> m = {{"int", 0}};
        auto da1 = new unsigned char[3000];
        auto da2 = da1;
        for (int i = 0, j = 0; i < length; i += 2) {
            auto dt = strings::trim(base[i]) + " " + strings::trim(base[i + 1]);
            da0[j++] = dt;
            auto p = strings::spilt(base[i + 1]);
            if (p[0] == "int") {
                int *pi = reinterpret_cast<int *>(da2);
                for (int i = 1; i < p.size(); i++) {
                    *(pi++) = std::stoi(p[i]);
                }
                da2 = reinterpret_cast<unsigned char *>(pi);
            }
            if (p[0] == "byte") {
                auto *p4 = reinterpret_cast<unsigned char *>(da2);
                for (int i = 1; i < p.size(); i++) {
                    *(p4++) = std::stoi(p[i]);
                }
                da2 = reinterpret_cast<unsigned char *>(p4);
            }
        }
        data_raw_len = length;
        return 0;
    }
};

#endif