//
// Created by dos DRTT on 10/25/20.
//

#ifndef MACHINE_BITMAP_HH
#define MACHINE_BITMAP_HH

#include <type_traits>

template<typename type>
class bitmap {
private:
    type *_map;
    unsigned long _size;
    unsigned long size;
public:
    explicit bitmap(unsigned long k = 64);

    bool clear();

    bool get_bit(unsigned long k);

    void set_bit(unsigned long k, bool b);

    int get_bit_false_set_true();
};

template<typename type>
bitmap<type>::bitmap(unsigned long k):size(k), _size((k / (sizeof(type) * 8)) + 1) {
    _map = new type[_size];
}


template<typename type>
bool bitmap<type>::clear() {
    memset(_map, 0, sizeof(type) * _size);
}

template<typename type>
bool bitmap<type>::get_bit(unsigned long k) {
    auto i = k / sizeof(type);
    auto g = k % sizeof(type);
    return (1ul << g) & _map[i];
}

template<typename type>
void bitmap<type>::set_bit(unsigned long k, bool b) {
    auto i = k / sizeof(type);
    auto g = k % sizeof(type);
    _map[i] = b ? (_map[i] | (1ul << g)) : (_map[i] & ~(1ul << g));
}

template<typename type>
int bitmap<type>::get_bit_false_set_true() {
    for (int i = 0; i < size; ++i) {
        if (!get_bit(i)) {
            set_bit(i, true);
            return i;
        }
    }
    return -1;
}

#endif //MACHINE_BITMAP_HH
