//
// Created by dos DRTT on 10/27/20.
//

#include "x86mmu.hh"
#include "../util/binary.hh"

namespace gunplan::cplusplus::machine {

    address_bond x86mmu::transfer(segment_selector sd, address_bond offset) {
        auto sdt = OperatorBitF(sd.key, 2) ? ldt : gdt;
        if (offset > ldt[sd.key].len) {
            throw offset;
        }
        return (sdt[sd.key >> 3u].start << 20u) + offset;
    }

    void x86mmu::set_ldt(segment_disruptor *ldt) {
        this->ldt = ldt;
    }

    void x86mmu::set_gdt(segment_disruptor *gdt) {
        this->gdt = gdt;
    }
}