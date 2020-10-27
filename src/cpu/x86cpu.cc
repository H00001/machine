

#include "x86cpu.hh"
#include "../proc/process.hh"
#include "x86mmu.hh"


namespace gunplan::cplusplus::machine {
    int x86cpu::push_process(int pid) {
        task_struct *next = process::get_process(pid);
        memmove(&tss, next, sizeof(m_cpu));
        tss.ldt_cache = next->ldt;
        return this->boot();
    }

    int x86cpu::boot() {
        while (true) {
            auto i = mm->fetch_instrument(x86mmu::transfer(tss.ldt_cache, segment_selector{tss.cs}, tss.rip));
            tss.pc = i;
            auto d = decode(tss.pc);
            if (execute(d) < 0) {
                break;
            }
        }
        return 0;
    }

    int x86cpu::execute(decode_result d) {
        push_stack(0);
        if (operMap.count(d.cmd) > 0) {
            auto fn = operMap[d.cmd];
            return fn(d.oplist);
        }
        throw -3;
    }

    // DE
    decode_result x86cpu::decode(data_bond pc) {
        return DE::decode(pc, &regHeap);
    }


    cpu_register x86cpu::pop_stack() {
        auto data = pop_stack(tss.ldt_cache, tss.ss, tss.esp);
        tss.esp--;
        return data;
    }


    void x86cpu::push_stack(unsigned long val) {
        push_stack(val, tss.ldt_cache, tss.ss, tss.esp);
        tss.esp++;
    }

    x86cpu::~x86cpu() {
        delete gdt;
    }

    x86cpu::x86cpu(memory *mm) : mm(mm) {

    }

    void x86cpu::set_resource(memory *mm) {
        this->mm = mm;

    }


    void x86cpu::push_stack(data_bond val, segment_disruptor *ldt, data_bond segment, data_bond offset) {
        mm->write(x86mmu::transfer(ldt, segment_selector{segment}, offset), val);
    }

    data_bond x86cpu::pop_stack(segment_disruptor *ldt, data_bond segment, data_bond offset) {
        return mm->read(x86mmu::transfer(ldt, segment_selector{segment}, offset - 1));
    }


}