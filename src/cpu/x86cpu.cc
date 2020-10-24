#pragma once

#include "x86cpu.hh"


namespace gunplan::cplusplus::machine {
    void x86cpu::push_process(std::pair<std::pair<code_buffer, data_buffer>, unsigned long> p) {
        process *proc = new process;
        int pid = proc->add_process(mm->load(p));
        task_struct *next = proc->getProcess(pid);
        memmove(&tss, next, sizeof(task_struct));
        this->execute(next->ldt);
    }

    void x86cpu::execute(segment_disruptor *ldt) {
        tss.ldt_cache = ldt;
        push_stack(32767);
        while (true) {
            tss.pc = memory::hd_code_mem[memory::transfer(tss.ldt_cache, segment_selector{tss.cs}, tss.rip)];
            if ((decode(tss.pc) != 0)) {
                break;
            }
        }
    }

    int x86cpu::decode(std::string basicString) {
        auto oper = stoi(strings::spilt(basicString)[0]);
        auto val = strings::spilt(basicString)[1];
        int r;
        auto o = decode0(val, &regHeap);
        if ((operMap.count(oper) == 1)) {
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
        printf("esp:%d\n", tss.esp);
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