//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_MEMORY_HH
#define MACHINE_MEMORY_HH

#include<string>
#include<fstream>
#include "../common/base.hh"
#include "../common/const.hh"
#include <map>
#include <regex>


namespace gunplan::cplusplus::machine {
    const unsigned long mm_size = 4 * 1024 * 1024;
    using address = unsigned long;

    struct segment_selector {
        unsigned long key;
    };

    struct logical_address {
        segment_selector sel;
        int offset;
    };


    class memory {

    private:
    public:
        static unsigned long *hd_mem;
        static std::string *hd_code_mem;


        void pushStack(unsigned long val, segment_disruptor *ldt, unsigned long segment, unsigned long offset) {
            memory::hd_mem[transfer(ldt, segment_selector{segment}, offset)] = val;
        }

        unsigned long popStack(segment_disruptor *ldt, unsigned long segment, unsigned long offset) {
            return memory::hd_mem[transfer(ldt, segment_selector{segment}, offset - 1)];
        }

        static segment_disruptor *load(std::pair<code_buffer, data_buffer> p) {
            for (int i = 0; i < p.first.length; ++i) {
                hd_code_mem[i] = p.first.b[i];
            }
            auto *t = new segment_disruptor[3];
            t[0] = segment_disruptor{transfer(0), p.first.length, 0};
            t[1] = segment_disruptor{transfer(0), p.second.length, 0};
            t[2] = segment_disruptor{transfer(0), 1024, 0};
            return t;
        }


        static address transfer(segment_disruptor *ldt, segment_selector sd, cpu_register offset) {
            if (offset > ldt[sd.key].len) {
                throw -1;
            }
            return (ldt[sd.key].start << 8) + offset;
        }

        static address transfer(address physics) {
            return physics << 8;
        }

    };


}
#endif //MACHINE_MEMORY_HH
