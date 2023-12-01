#include "balloc.hpp" // Assuming this is the header file containing the Heap class
#include "r_balloc.hpp"

#include "simpletest/simpletest.h"
#include <cstddef>
#include <iostream>
#include <ostream>

#define is_aligned(POINTER, BYTE_COUNT)                                        \
    (((unsigned long)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

char const *groups[] = {"BumpUp", "BumpDown"};

DEFINE_TEST_G(Test1, BumpDown) {
    // Test 1: Allocate memory successfully
    BumpDown<20 * sizeof(int)> bumper;
    int *x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");

    // Test 2: Allocate more memory successfully
    int *y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 2,
                 "Incorrect number of allocations");

    // Test 3: Fail to allocate due to insufficient memory
    int *z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 2,
                 "Incorrect number of allocations");
}

DEFINE_TEST_G(Test2, BumpDown) {
    BumpDown<20 * sizeof(char)> bumper;
    char *x = bumper.alloc<char>(20);
    TEST_MESSAGE(x != nullptr, "Failed to allocate");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");
    char *y = bumper.alloc<char>(1);
    TEST_MESSAGE(y == nullptr, "Should have failed to allocate");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");
    bumper.dealloc();
    TEST_MESSAGE(bumper.get_num_allocations() == 0,
                 "Incorrect number of allocations");
    y = bumper.alloc<char>(20);
    TEST_MESSAGE(y != nullptr, "Failed to allocate after deallocation!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");
}

DEFINE_TEST_G(Test3, BumpDown) {
    BumpDown<64 * sizeof(char)> B;
    char *a1 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a1, alignof(char)),
                 "Alignment is incorrect for this type");
    short *a2 = B.alloc<short>(1);
    TEST_MESSAGE(is_aligned(a2, alignof(short)),
                 "Alignment is incorrect for this type");
    char *a3 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a3, alignof(char)),
                 "Alignment is incorrect for this type");
    int *a4 = B.alloc<int>(1);
    TEST_MESSAGE(is_aligned(a4, alignof(int)),
                 "Alignment is incorrect for this type");
    char *a5 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a5, alignof(char)),
                 "Alignment is incorrect for this type");
    char *a6 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a6, alignof(char)),
                 "Alignment is incorrect for this type");
}
DEFINE_TEST_G(Test4, BumpDown) {
    // Test 1: Allocate memory successfully
    BumpDown<20 * sizeof(int)> bumper;
    int *x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");

    // Test 2: Allocate more memory successfully
    int *y = bumper.alloc<int>(11);
    TEST_MESSAGE(y == nullptr, "Should have failed to allocate!!!!");

    // Test 3: Fail to allocate due to insufficient memory
    int *z = bumper.alloc<int>(10);
    TEST_MESSAGE(z != nullptr, "Failed to allocate!!!!");
}

DEFINE_TEST_G(Test1, BumpUp) {
    // Test 1: Allocate memory successfully
    BumpUp<20 * sizeof(int)> bumper;
    int *x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");

    // Test 2: Allocate more memory successfully
    int *y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 2,
                 "Incorrect number of allocations");

    // Test 3: Fail to allocate due to insufficient memory
    int *z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 2,
                 "Incorrect number of allocations");
}

DEFINE_TEST_G(Test2, BumpUp) {
    BumpUp<20 * sizeof(char)> bumper;
    char *x = bumper.alloc<char>(20);
    TEST_MESSAGE(x != nullptr, "Failed to allocate");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");
    char *y = bumper.alloc<char>(1);
    TEST_MESSAGE(y == nullptr, "Should have failed to allocate");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");
    bumper.dealloc();
    TEST_MESSAGE(bumper.get_num_allocations() == 0,
                 "Incorrect number of allocations");
    y = bumper.alloc<char>(20);
    TEST_MESSAGE(y != nullptr, "Failed to allocate after deallocation!!!!");
    TEST_MESSAGE(bumper.get_num_allocations() == 1,
                 "Incorrect number of allocations");
}

DEFINE_TEST_G(Test3, BumpUp) {
    BumpUp<64 * sizeof(char)> B;
    char *a1 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a1, alignof(char)),
                 "Alignment is incorrect for this type");
    short *a2 = B.alloc<short>(1);
    TEST_MESSAGE(is_aligned(a2, alignof(short)),
                 "Alignment is incorrect for this type");
    char *a3 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a3, alignof(char)),
                 "Alignment is incorrect for this type");
    int *a4 = B.alloc<int>(1);
    TEST_MESSAGE(is_aligned(a4, alignof(int)),
                 "Alignment is incorrect for this type");
    char *a5 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a5, alignof(char)),
                 "Alignment is incorrect for this type");
    char *a6 = B.alloc<char>(1);
    TEST_MESSAGE(is_aligned(a6, alignof(char)),
                 "Alignment is incorrect for this type");
}
DEFINE_TEST_G(Test4, BumpUp) {
    // Test 1: Allocate memory successfully
    BumpUp<20 * sizeof(int)> bumper;
    int *x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");

    // Test 2: Allocate more memory successfully
    int *y = bumper.alloc<int>(11);
    TEST_MESSAGE(y == nullptr, "Should have failed to allocate!!!!");

    // Test 3: Fail to allocate due to insufficient memory
    int *z = bumper.alloc<int>(10);
    TEST_MESSAGE(z != nullptr, "Failed to allocate!!!!");
}

int main() {
    bool pass = true;

    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }

    return pass ? 0 : 1;
}
