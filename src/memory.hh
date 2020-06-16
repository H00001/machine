//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_MEMORY_HH
#define MACHINE_MEMORY_HH

#include<string>
#include<fstream>
#include "base.hh"
#include <map>


namespace gunplan::cplusplus::machine {
    struct seg {
        std::string *code;
        int clen;
        byte *data;
        int dlen;
    };


    class memory {
    private:
    public:
        seg *load(const std::string &file) {
            int pos = -1;
            auto *buff = new std::string[120];
            byte *data = new byte[2000];
            std::ifstream in(file);
            std::string line;
            int ldata = 0;
            int lcode = 0;
            while (getline(in, line)) {
                if (line == "@data") {
                    pos = 0;
                } else if (line == "@code") {
                    pos = 1;
                } else if (pos == 0) {
                    ldata++;
                } else if (pos == 1) {
                    buff[lcode] = line;
                    lcode++;
                }
            }


            seg *s = new seg{buff, lcode, data, ldata};

            return s;
        }


        template<typename tp>
        static tp transfer(std::map<cpu_register, LDT *> *ldtl, cpu_register seg, cpu_register mov) {
            return (tp) (*ldtl)[seg]->base + mov;
        }

    };

}

#endif //MACHINE_MEMORY_HH
