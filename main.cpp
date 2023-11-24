#include "balloc.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>

int main(int argc, char *argv[]) {
    Bump<64 * sizeof(char)> B;
    B.alloc<char>(1);
    B.alloc<short>(1);
    B.alloc<char>(1);
    B.alloc<int>(1);
    B.alloc<char>(1);
    B.alloc<char>(1);

    // while (auto x = B.alloc<char>(5)) {
    //     std::cout << B.get_num_allocations() << std::endl;
    // }
    //
    // while (B.get_num_allocations()) {
    //     B.dealloc();
    // }
    // while (auto x = B.alloc<int>(5)) {
    //     std::cout << B.get_num_allocations() << std::endl;
    // }

    return 0;
}
