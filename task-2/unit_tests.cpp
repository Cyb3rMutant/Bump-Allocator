#include "balloc.hpp"

#include "../simpletest/simpletest.h"

char const *groups[] = {
    "Bump",
};

DEFINE_TEST_G(BumTest1, Bump) {
    Heap<20 * sizeof(int)> bumper;
    int *x = bumper.alloc<int>(10);
    TEST_MESSAGE(x != nullptr, "Failed to allocate!!!!");
    int *y = bumper.alloc<int>(10);
    TEST_MESSAGE(y != nullptr, "Failed to allocate!!!!");
    int *z = bumper.alloc<int>(10);
    TEST_MESSAGE(z == nullptr, "Should have failed to allocate!!!!");
}

int main() {
    bool pass = true;

    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }

    return pass ? 0 : 1;
}
