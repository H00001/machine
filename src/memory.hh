//
// Created by dos DRTT on 6/13/20.
//

#ifndef MACHINE_MEMORY_HH
#define MACHINE_MEMORY_HH

#include<string>
#include<fstream>
#include "base.hh"
#include <map>
#include "util.hh"


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
        byte *anaData(std::string *s, int n) {
            auto *data_table = new std::map<std::string, int>;
            byte *data = new byte[2000];
            int pos = 0;
            for (int i = 0; i < n; i += 2) {
                (*data_table)[s[i]] = pos;
                auto dt = s[i + 1];
                auto o = strings::spilt(dt, " ");
                if (o[0] == "int") {
                    auto r = strings::Analyze(o[1], nullptr);
                    pos += r->size() * sizeof(int);
                    int *p = reinterpret_cast<int *>(data);
                    for (auto &v :*r) {
                        *p = v->operval;
                        p++;
                    }
                }
            }
            return data;
        }

        seg *load(const std::string &file) {
            int pos = -1;
            auto *buff = new std::string[120];
            auto *d_buff = new std::string[120];
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
                    d_buff[ldata] = line;
                    ldata++;
                } else if (pos == 1) {
                    buff[lcode] = line;
                    lcode++;
                }
            }
            byte *data = anaData(d_buff, ldata);

            seg *s = new seg{buff, lcode, data, ldata};

            return s;
        }


        template<typename tp>
        static tp transfer(std::map<cpu_register, LDT *> *ldtl, cpu_register seg, cpu_register mov) {
            if (mov > (*ldtl)[seg]->len) {
                throw -1;
            }
            return (tp) (*ldtl)[seg]->base + mov;
        }

    };

}

#endif //MACHINE_MEMORY_HH
