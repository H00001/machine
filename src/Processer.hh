//
// Created by dos DRTT on 6/14/20.
//

#ifndef MACHINE_PROCESSER_HH
#define MACHINE_PROCESSER_HH

#endif //MACHINE_PROCESSER_HH

#include <string>
#include <map>
#include "base.hh"

class Processer {
public:
    virtual std::string operatorName() = 0;

    virtual int operatorCode() = 0;

    virtual void deal(m_cpu *c_register, std::map<std::string, unsigned int *> rrgHeap) = 0;
};