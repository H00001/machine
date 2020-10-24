//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_PROGRAM_COMPILE_HH_1
#define MACHINE_PROGRAM_COMPILE_HH_1

#include <utility>
#include <string>
#include "../common/const.hh"


class program_compile {
protected:
    using buf = std::pair<code_buffer, data_buffer>;
public:
    virtual buf compile_load(std::string file_name) = 0;

    virtual unsigned int compile(buf) = 0;

    virtual void rewrite_to_file(std::string file_name) = 0;

    virtual ~program_compile() = default;
};


#endif //MACHINE_PROGRAM_COMPILE_HH