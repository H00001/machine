#include <iostream>
#include "src/personal_computer.hh"

int main() {
    personal_computer p(new memory, new x86cpu(), new program_compile_x86());
    p.boot();
    p.launch("/Users/dosdrtt/CLionProjects/Machine/instr");
    p.destroy();
    return 0;
}
