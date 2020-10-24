//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PC_HH
#define MACHINE_PC_HH

#include <utility>

#include "memory.hh"
#include "process.hh"
#include "cpu.hh"
#include "compile/program_compile.hh"
#include "compile/program_compile_x86.hh"

using namespace gunplan::cplusplus::machine;

class pc {
private:
    memory *mm{};
    process *p{};
    cpu *c;
    program_compile *compile;
public:
    pc() {
        c = new cpu(new memory(), new process);
    }

    virtual ~pc() {
        delete c;
    }

    void boot(const std::string &filename) {
        compile = new program_compile_x86();
        auto d = compile->compile_load(filename);
        auto ip = compile->compile(d);
        compile->rewrite_to_file("/tmp/data", d.first.b, d.first.length);
        c->PushProcess(std::pair<std::pair<code_buffer, data_buffer>, unsigned long>(d, ip));
    }
};

#endif //MACHINE_PC_HH
