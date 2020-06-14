//
// Created by dos DRTT on 6/14/20.
//

#ifndef MACHINE_MOVPROCESSER_HH
#define MACHINE_MOVPROCESSER_HH


#include "Processer.hh"

class MovProcesser : public Processer {
    std::string operatorName() {
        return "mov";
    }

    int operatorCode() {
        return 1;
    }

    void deal(m_cpu *c_register, std::map<std::string, unsigned int *> rrgHeap) {

    }
};


#endif //MACHINE_MOVPROCESSER_HH
