//
// Created by dos DRTT on 10/26/20.
//

#ifndef MACHINE_WRITE_INSTRUMENT_HH
#define MACHINE_WRITE_INSTRUMENT_HH


#include "standard.hh"
#include "../util/binary.hh"

auto write_0p_is_num = [](unsigned int *o) {
    *o = (*o) | (1 << 1);
};

auto write_1p_is_num = [](unsigned int *o) {
    *o = (*o) | (1 << 0);
};

auto write_p_is_2 = [](unsigned int *o) {
    *o = (*o) | (1 << 2);
};

class write_instrument {
public:
    static void write_op_type(word *instr, op_len op);

    static void write_p0(word *instr, op_len op, bool isNum);

    static void write_p1(word *instr, op_len op, bool isNum);


};


#endif //MACHINE_WRITE_INSTRUMENT_HH
