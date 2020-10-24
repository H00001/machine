//
// Created by dos DRTT on 10/24/20.
//

#include <string>
#include <map>
#include <list>

#ifndef MACHINE_COMMON_HH
#define MACHINE_COMMON_HH

#endif //MACHINE_COMMON_HH

namespace gunplan::cplusplus::machine {
    enum operaType {
        num, reg, str
    };

    struct oper_code {
        unsigned long *oper_reg;
        std::string oper_str;
        std::string oper_reg_name;
        long oper_val{};
        operaType oper_type;
    };

    using opFN = std::function<int(std::list<oper_code *> *)>;
    using rHeap = std::map<unsigned short, unsigned long *>;
    using operatorMap = std::map<int, opFN>;
}