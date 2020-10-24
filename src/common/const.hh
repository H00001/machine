//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_CONST_HH
#define MACHINE_CONST_HH

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

#endif //MACHINE_CONST_HH
