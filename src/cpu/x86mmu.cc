//
// Created by dos DRTT on 10/27/20.
//

#include "x86mmu.hh"

namespace gunplan::cplusplus::machine {
    address_bond x86mmu::transfer(segment_selector sd, address_bond offset) {
        if (offset > ldt[sd.key].len) {
            throw offset;
        }
        return (ldt[sd.key].start << 20u) + offset;
    }

    void x86mmu::set_ldt(segment_disruptor *ldt) {
        this->ldt = ldt;
    }
}