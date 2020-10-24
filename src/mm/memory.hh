//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_MEMORY_HH
#define MACHINE_MEMORY_HH

#include<string>
#include<fstream>
#include "../base.hh"
#include "../common/const.hh"
#include <map>
#include <regex>


namespace gunplan::cplusplus::machine {
    const unsigned long mm_size = 16 * 1024 * 1024;
    using address = unsigned long;

    struct seg {
        unsigned long code;
        int code_len;
        unsigned long data;
        int data_len;
        unsigned long stack;
        int stack_len;
        unsigned long ip;
    };


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

        seg *load(std::pair<std::pair<code_buffer, data_buffer>, unsigned long> p) {
            for (int i = 0; i < p.first.first.length; ++i) {
                hd_code_mem[i] = p.first.first.b[i];
            }
            return new seg{0, p.first.first.length, 0, p.first.second.length, 1024, 1024, p.second};
        }


        static address transfer(segment_disruptor *ldt, segment_selector sd, cpu_register offset) {
            if (offset > ldt[sd.key].len) {
                throw -1;
            }
            return (ldt[sd.key].start << 8) + offset;
        }

    };


}
#endif //MACHINE_MEMORY_HH
