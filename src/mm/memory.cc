//
// Created by dos DRTT on 10/24/20.
//

#include "memory.hh"

namespace gunplan::cplusplus::machine {


    memory::memory(int mb) : hd_mem(new address_bond[mb * M]), hd_code_mem(new std::string[mb * M]) {
        mem_manager = new bitmap<address_bond>(mb);
    }

    memory::~memory() {
        delete[] hd_mem;
        delete[] hd_code_mem;
        delete mem_manager;
    }


    segment_disruptor *memory::load(std::pair<code_buffer, data_buffer> p) {
        data_bond bit = mem_manager->get_bit_false_set_true();
        if (bit < 0) {
            return nullptr;
        }
        auto base = hd_code_mem + (bit << 20);
        for (int i = 0; i < p.first.length; ++i) {
            base[i] = p.first.b[i];
        }
        auto *t = new segment_disruptor[3];
        t[0] = segment_disruptor{bit, p.first.length, 0};
        t[1] = segment_disruptor{bit, p.second.length, 0};
        t[2] = segment_disruptor{0, 1024, 0};
        return t;

    }


    void memory::write(address_bond addr, data_bond data) {
        this->hd_mem[addr] = data;
    }

    data_bond memory::read(address_bond addr) {
        return this->hd_mem[addr];
    }

    std::string memory::fetch_instrument(physics_address addr) {
        return hd_code_mem[addr];
    }

}