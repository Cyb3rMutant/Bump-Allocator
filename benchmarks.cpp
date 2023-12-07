#include <allocators/balloc.hpp>
#include <allocators/r_balloc.hpp>
#include <benchmark.hpp>
#include <cstdint>

struct MyStruct {
    double a;
    double a1;
    double a2;
    double a3;
};

void test_up_small() {
    BumpUp<sizeof(int) * 10000> b;

    for (int i = 0; i < 5; i++) {
        while (b.alloc<int>(1)) {
            b.alloc<char>(1);
            b.alloc<short>(1);
            b.alloc<char>(1);
        }
        b.force_dealloc();
    }
}

void test_down_small() {
    BumpDown<sizeof(int) * 10000> b;

    for (int i = 0; i < 5; i++) {
        while (b.alloc<int>(1)) {
            b.alloc<char>(1);
            b.alloc<short>(1);
            b.alloc<char>(1);
        }
        b.force_dealloc();
    }
}

void test_up_big() {
    BumpUp<sizeof(int) * 10000> b;

    for (int i = 0; i < 5; i++) {
        while (b.alloc<MyStruct>(1))
            b.alloc<char>(1);
        b.force_dealloc();
    }
}

void test_down_big() {
    BumpDown<sizeof(int) * 10000> b;

    for (int i = 0; i < 5; i++) {
        while (b.alloc<MyStruct>(1))
            b.alloc<char>(1);
        b.force_dealloc();
    }
}

char *old_alignement(char *ptr) {
    char *new_ptr = ptr;
    if (unsigned long remainder = ((unsigned long)ptr % alignof(int))) {
        new_ptr += alignof(int) - remainder;
    }
    return new_ptr;
}

char *new_alignement(char *ptr) {
    char *new_ptr = reinterpret_cast<char *>(
        (reinterpret_cast<uintptr_t>(ptr) - 1u + alignof(int)) & -alignof(int));
    return new_ptr;
}

void test_dealloc() {
    BumpDown<1024> b;
    for (int i = 0; i < 400; i++) {
        b.alloc<int>(256);
        b.dealloc();
    }
}

void test_destruct() {
    for (int i = 0; i < 400; i++) {
        BumpDown<1024> b;
        b.alloc<int>(256);
    }
}

int main() {
    {
        Benchmark b(5000);
        b.benchmark("up small obj", test_up_small);
        b.benchmark("down small obj", test_down_small);
        b.print();
    }
    {
        Benchmark b(5000);
        b.benchmark("up big obj", test_up_big);
        b.benchmark("down big obj", test_down_big);
        b.print();
    }
    {
        Benchmark b(5000);
        b.benchmark("new alignement", new_alignement,
                    reinterpret_cast<char *>(0x12345678));
        b.benchmark("old alignement", old_alignement,
                    reinterpret_cast<char *>(0x12345678));
        b.print();
    }
    {
        Benchmark b(5000);
        b.benchmark("dealloc", test_dealloc);
        b.benchmark("destructor", test_destruct);
        b.print();
    }

    return 0;
}
