//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PERSONAL_COMPUTER_HH
#define MACHINE_PERSONAL_COMPUTER_HH

#include <utility>

#include "mm/memory.hh"
#include "process.hh"
#include "cpu/cpu1.hh"
#include "compile/program_compile_x86.hh"
#include "cpu/x86cpu.hh"

using namespace gunplan::cplusplus::machine;


class personal_computer {
private:
    memory *mm;
    process *p{};
    cpu1 *c;
    program_compile *compile;
public:
    personal_computer(memory *mm, cpu1 *c, program_compile *comp)
            : mm(mm), c(c), compile(comp) {}

    virtual ~personal_computer() {
        delete c;
        delete mm;
        delete compile;
    }

    void boot() {
        c->set_resource(mm);
    }

    void launch(const std::string &filename) const {
        auto d = compile->compile_load(filename);
        auto ip = compile->compile(d);
        compile->rewrite_to_file("/Users/dosdrtt/tmp/data", d.first.b, d.first.length);
        c->push_process(std::pair<std::pair<code_buffer, data_buffer>, unsigned long>(d, ip));
    }

    void destroy() {
    }
};

#endif //MACHINE_PERSONAL_COMPUTER_HH
