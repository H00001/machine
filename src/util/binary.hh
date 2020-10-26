//
// Created by dos DRTT on 6/15/20.
//

#ifndef MACHINE_BINARY_HH
#define MACHINE_BINARY_HH


auto OperatorBitG = [](unsigned long x, short p, bool b) {
    unsigned long mv = 1 << p;
    return b ? (x | mv) : (x & ~(mv));
};

auto OperatorBitF = [](unsigned long x, short p) -> bool {
    return (x & (1 << p));
};

auto OperatorBiF0 = [](unsigned long x) {
    return OperatorBitF(x, 0);
};

auto OperatorBitF1 = [](unsigned long x) {
    return OperatorBitF(x, 1);
};

auto write_0_8 = [](unsigned int *o, unsigned char i) {
    unsigned int l = i;
    *o = (*o) | (l << 24);
};

auto write_1_8 = [](unsigned int *o, unsigned char i) {
    unsigned int l = i;
    *o = (*o) | (l << 16);
};


auto write_2_8 = [](unsigned int *o, unsigned char i) {
    unsigned int l = i;
    *o = (*o) | (l << 8);
};

auto write_3_8 = [](unsigned int *o, unsigned char i) {
    unsigned int l = i;
    *o = (*o) | (l << 0);
};


#endif //MACHINE_BINARY_HH

