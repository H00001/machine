//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_PROCESS_HH
#define MACHINE_PROCESS_HH


#include <string>
#include "../mm/memory.hh"
#include "../cpu/x86cpu.hh"

#define segment_selector_b short int
namespace gunplan::cplusplus::machine {

    using buf = std::tuple<segment_buffer, segment_buffer, unsigned int>;
    struct task_struct {
        m_cpu m;
        segment_disruptor *ldt{};
        int pid;
    };


    class process {
        /**
         * segment selector
        **/
        const segment_selector_b seg_code_selector = 0b100u;
        const segment_selector_b seg_data_selector = 0b1100u;
        const segment_selector_b seg_stack_selector = 0b10100u;
        const segment_selector_b seg_ex_selector = 0b11100;

    private:
        buf *task;
        int pid;
    public:
        explicit process(buf *task);

        int add_process(memory *mm);

        static task_struct *get_process(int id);

        virtual ~process();
    };


}


#endif //MACHINE_PROCESS_HH
