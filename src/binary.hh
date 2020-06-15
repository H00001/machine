//
// Created by dos DRTT on 6/15/20.
//

#ifndef MACHINE_BINARY_HH
#define MACHINE_BINARY_HH

#endif //MACHINE_BINARY_HH

auto OperatorBitG = [](unsigned long x, short p, bool b) {
    unsigned long mv = 1ul << p;
    return b ? (x | mv) : (x & ~(mv));
};

auto OperatorBitF = [](unsigned long x, short p) -> bool {
    return (x & (1 << p));
};

auto OperatorBitF0 = [](unsigned long x) {
    return OperatorBitF(x, 0);
};

auto OperatorBitF1 = [](unsigned long x) {
    return OperatorBitF(x, 1);
};

