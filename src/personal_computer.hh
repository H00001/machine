//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PERSONAL_COMPUTER_HH
#define MACHINE_PERSONAL_COMPUTER_HH

#include <utility>

#include "mm/memory.hh"
#include "cpu/cpu1.hh"
#include "compile/program_compile_x86.hh"
#include "cpu/x86cpu.hh"
#include "proc/process.hh"

using namespace gunplan::cplusplus::machine;


class personal_computer {
private:
    memory *mm;
    cpu1<cpu_register> *c;
    program_compile *compile;
public:
    personal_computer(memory *mm, cpu1<cpu_register> *c, program_compile *comp)
            : mm(mm), c(c), compile(comp) {}

    virtual ~personal_computer() {
        delete c;
        delete mm;
        delete compile;
    }

    void init() {
        c->set_resource(mm);
    }

    [[nodiscard]] int launch(const std::string &filename) const {
        auto d = compile->compile_load(filename);
        compile->rewrite_to_file("/Users/dosdrtt/tmp/data");
        auto *ps = new process(&d);
        auto r = c->push_process(ps->add_process(mm));
        delete ps;
        return r;
    }

};

#endif //MACHINE_PERSONAL_COMPUTER_HH
