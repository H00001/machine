

#include "x86cpu.hh"

namespace gunplan::cplusplus::machine {
    int x86cpu::push_process(int pid) {
        task_struct *next = process::get_process(pid);
        memmove(&tss, next, sizeof(m_cpu));
        mmu.set_ldt(next->ldt);
        return this->boot(next->ldt[0].len);
    }

    int x86cpu::boot(int k) {
        push_stack(k);
        while (true) {
            // FI
            tss.pc = mm->fetch_instrument(mmu.transfer(segment_selector{tss.cs}, tss.rip));
            if (tss.pc == 0) {
                return tss.ecx;
            }
            // DE
            auto data_to_execute = decode(tss.pc);
            // EX
            auto data_to_write = execute(data_to_execute);
            // WB
            tss.rip = data_to_write.next_add ? tss.rip + 1 : data_to_write.next;
            auto dr = write_back(data_to_write.w);

        }
    }

    ex_ret x86cpu::execute(decode_result d) {
        if (operMap.count(d.cmd) > 0) {
            auto fn = operMap[d.cmd];
            return fn(d.oplist);
        }
        throw nullwb;
    }

    // DE
    decode_result x86cpu::decode(data_bond pc) {
        return DE::decode(pc, &regHeap);
    }


    cpu_register x86cpu::pop_stack() {
        auto data = pop_stack(tss.ss, tss.esp);
        tss.esp--;
        return data;
    }


    void x86cpu::push_stack(data_bond val) {
        push_stack(val, tss.ss, tss.esp);
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


    void x86cpu::push_stack(data_bond val, cpu_register_segment segment, data_bond offset) {
        mm->write(mmu.transfer(segment_selector{segment}, offset), val);
    }

    data_bond x86cpu::pop_stack(cpu_register_segment segment, data_bond offset) {
        return mm->read(mmu.transfer(segment_selector{segment}, offset - 1));
    }

    int x86cpu::write_back(wb_data data) {
        if (!data.first) {
            return 0;
        }
        *data.second.first = data.second.second;
    }


}