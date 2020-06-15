//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_MEMORY_HH
#define MACHINE_MEMORY_HH

#include<string>
#include<fstream>

class memory {
public:
    std::string *load(const std::string &file) {
        auto *buff = new std::string[120];
        std::ifstream in(file);
        std::string line;
        int i = 0;
        while (getline(in, line)) {
            buff[i] = line;
            i++;
        }
        return buff;
    }
};


#endif //MACHINE_MEMORY_HH
