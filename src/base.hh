//
// Created by dos DRTT on 6/14/20.
//

#ifndef MACHINE_BASE_HH
#define MACHINE_BASE_HH

#endif //MACHINE_BASE_HH

#include <string>
#include <map>

using cpu_register = unsigned long;
using intstru_len = int;

struct m_cpu {
    std::string *cs;
    cpu_register *ss;
    cpu_register ds;

    cpu_register esp;
    cpu_register ebp;

    cpu_register eax;
    cpu_register ebx;
    cpu_register ecx;
    cpu_register edx;

    cpu_register esi;
    cpu_register edi;

    cpu_register rs0;
    cpu_register rs1;
    cpu_register rs2;

    std::map<std::string, int> *flages;

    unsigned long rip;
    std::string pc;
};
