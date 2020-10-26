//
// Created by dos DRTT on 10/24/20.
//

#ifndef MACHINE_PROGRAM_COMPILE_HH_1
#define MACHINE_PROGRAM_COMPILE_HH_1

#include <utility>
#include <string>
#include "standard.hh"


class program_compile {
protected:
public:
    virtual ret compile_load(std::string file_name) = 0;

    virtual void rewrite_to_file(std::string file_name) = 0;

    virtual ~program_compile() = default;
};


#endif //MACHINE_PROGRAM_COMPILE_HH
