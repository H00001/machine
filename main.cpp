#include <iostream>
#include "src/personal_computer.hh"

int main() {
    personal_computer p(new memory(16), new x86cpu(), new program_compile_x86());
    p.init();
    p.launch("/Users/dosdrtt/CLionProjects/Machine/instr");
    p.launch("/Users/dosdrtt/CLionProjects/Machine/instr");
    //  p.launch("/Users/dosdrtt/CLionProjects/Machine/instr");
    return 0;
}
