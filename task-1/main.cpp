#include "balloc.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>

int main(int argc, char *argv[]) {
    Heap<64 * sizeof(char)> B;
    while (auto x = B.alloc<char>(5)) {
        std::cout << B.get_num_allocations() << std::endl;
    }

    while (B.get_num_allocations()) {
        B.dealloc();
    }
    while (auto x = B.alloc<int>(5)) {
        std::cout << B.get_num_allocations() << std::endl;
    }

    return 0;
}
