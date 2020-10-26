//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_MEMORY_HH
#define MACHINE_MEMORY_HH

#include<string>
#include<fstream>
#include "../common/base.hh"
#include "../common/const.hh"
#include "../util/bitmap.hh"
#include <map>
#include <regex>

#define M 1024*1024
#define K 1024
namespace gunplan::cplusplus::machine {
    using data_bond = unsigned int;
    using address_bond = unsigned int;
    using physics_address = address_bond;

    struct segment_selector {
        unsigned long key;
    };

    class memory {
    private:
        address_bond *const hd_mem;
        std::string *const hd_code_mem;
        bitmap<address_bond> *mem_manager;
    public:


        // create a memory
        explicit memory(int mb);

        std::string fetch_instrument(physics_address addr);

        void write(address_bond, data_bond data);

        segment_disruptor *load(std::pair<code_buffer, data_buffer> p);

        data_bond read(address_bond addr);

        virtual ~memory();
    };

}
#endif //MACHINE_MEMORY_HH
