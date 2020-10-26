//
// Created by dos DRTT on 10/24/20.
//
#ifndef MACHINE_PROGRAM_COMPILE_X86_HH
#define MACHINE_PROGRAM_COMPILE_X86_HH

#include <fstream>
#include <map>
#include <vector>
#include "program_compile.hh"
#include "RelocatdFilter.hh"
#include "../util/strings.hh"

class program_compile_x86 : public program_compile {

private:
    Product *p = new Product({new RegisterFilter(), new AddressFilter(), new NumberFilter()});
    InstrumentFilter *inf = new InstrumentFilter();
    word *b;
    int len;
public:

    program_compile_x86() = default;

    ret compile_load(std::string file_name) override;

    void rewrite_to_file(std::string file_name) override;

    virtual ~program_compile_x86() override;

private:
    unsigned int compile_code_segment(std::string *base, int length, word *lbuf, int &ddlen);

    unsigned int compile_data_segment(std::string *base, int length, word *, int &) {
        return 0;
    }
};

#endif