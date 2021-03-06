//
// Created by dos DRTT on 10/26/20.
//

#include "write_instrument.hh"

void write_instrument::write_op_type(word *instr, op_len op) {
    write_0_8(instr, op);
}

void write_instrument::write_p0(word *instr, op_len op, bool isNum, bool isAddr) {
    write_1_8(instr, op);
    if (isNum) {
        write_0p_is_num(instr);
    }
    if (isAddr) {
        write_0p_is_addr(instr);
    }
    write_p_is_1(instr);
}

void write_instrument::write_p1(word *instr, op_len op, bool isNum, bool isAddr) {
    write_2_8(instr, op);
    if (isNum) {
        write_1p_is_num(instr);
    }
    if (isAddr) {
        write_1p_is_addr(instr);
    }
    write_p_is_2(instr);
}