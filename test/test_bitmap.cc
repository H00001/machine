#include "../src/util/bitmap.hh"
#include <iostream>

using namespace std;

int main() {
    bitmap<unsigned long> bm;
    bm.set_bit(10, true);
    cout << bm.get_bit(10) << endl;
}