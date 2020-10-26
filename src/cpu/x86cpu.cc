

#include "x86cpu.hh"
#include "../proc/process.hh"


namespace gunplan::cplusplus::machine {
    int x86cpu::push_process(int pid) {
        task_struct *next = process::get_process(pid);
        memmove(&tss, next, sizeof(m_cpu));
        tss.ldt_cache = next->ldt;
        return this->execute();
    }

    int x86cpu::execute() {
        push_stack(32767);
        while (true) {
            tss.pc = mm->fetch_instrument(transfer(tss.ldt_cache, segment_selector{tss.cs}, tss.rip));
            if ((decode(tss.pc) != 0)) {
                break;
            }
        }
        return 0;
    }

    int x86cpu::decode(std::string basicString) {
        auto ss = strings::spilt(basicString);
        auto oper = stoi(ss[0]);
        auto val = ss[1];
        int r = 0;
        auto o = decode0(val, &regHeap);
        if (operMap.count(oper) == 1) {
            r = operMap[oper](o);
        }
        delete o;
        return r;
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
        mm->write(transfer(ldt, segment_selector{segment}, offset), val);
    }

    data_bond x86cpu::pop_stack(segment_disruptor *ldt, data_bond segment, data_bond offset) {
        return mm->read(transfer(ldt, segment_selector{segment}, offset - 1));
    }

    address_bond x86cpu::transfer(segment_disruptor *ldt, segment_selector sd, address_bond offset) {
        if (offset > ldt[sd.key].len) {
            throw offset;
        }
        return (ldt[sd.key].start << 20) + offset;
    }

}