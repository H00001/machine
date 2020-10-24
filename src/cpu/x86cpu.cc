

#include "x86cpu.hh"
#include "../proc/process.hh"


namespace gunplan::cplusplus::machine {
    int x86cpu::push_process(int pid) {
        task_struct *next = process::get_process(pid);
        memmove(&tss, next, sizeof(task_struct));
        tss.ldt_cache = next->ldt;
        return this->execute();
    }

    int x86cpu::execute() {
        push_stack(32767);
        while (true) {
            tss.pc = memory::hd_code_mem[memory::transfer(tss.ldt_cache, segment_selector{tss.cs}, tss.rip)];
            if ((decode(tss.pc) != 0)) {
                break;
            }
        }
        return 0;
    }

    int x86cpu::decode(std::string basicString) {
        auto oper = stoi(strings::spilt(basicString)[0]);
        auto val = strings::spilt(basicString)[1];
        int r = 0;
        auto o = decode0(val, &regHeap);
        if (operMap.count(oper) == 1) {
            r = operMap[oper](o);
        }
        delete o;
        return r;
    }


    unsigned long x86cpu::pop_stack() {
        auto data = mm->popStack(tss.ldt_cache, tss.ss, tss.esp);
        tss.esp--;
        return data;
    }


    void x86cpu::push_stack(unsigned long val) {
        mm->pushStack(val, tss.ldt_cache, tss.ss, tss.esp);
        tss.esp++;
    }

    x86cpu::~x86cpu() {
        delete mm;
        delete gdt;
    }

    x86cpu::x86cpu(memory *mm) : mm(mm) {

    }

    void x86cpu::set_resource(memory *mm) {
        this->mm = mm;
    }

}