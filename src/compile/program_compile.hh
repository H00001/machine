//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_PROGRAM_COMPILE_HH
#define MACHINE_PROGRAM_COMPILE_HH

#include <utility>
#include <string>

const int mm_size = 1024;
using byte = unsigned char;
using offset = unsigned int;


struct code_buffer {
    std::string *b;
    int length;
};

struct data_buffer {
    byte *b;
    int length;
};

class program_compile {
public:
    virtual std::pair<code_buffer, data_buffer> compile_load(std::string file_name) = 0;

    virtual unsigned int compile(std::pair<code_buffer, data_buffer>) = 0;

    virtual void rewrite_to_file(std::string file_name, std::string *b, int len) = 0;

};


#endif //MACHINE_PROGRAM_COMPILE_HH
