//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PC_HH
#define MACHINE_PC_HH

#include <utility>

#include "memory.hh"
#include "process.hh"
#include "cpu.hh"

class pc {
private:
    memory *mm;
    process *p;
    cpu *c;
public:
    pc() {
        c = new cpu(new memory(), new process);
    }

    ~pc() {
        delete c;
    }

    void boot(std::string filename) {
        c->PushProcess(filename);
    }
};


#endif //MACHINE_PC_HH