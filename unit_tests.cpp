#include <allocators/balloc.hpp>
#include <allocators/r_balloc.hpp>

#include <cstddef>
#include <iostream>
#include <ostream>
#include <simpletest/simpletest.h>

#define is_aligned(POINTER, BYTE_COUNT)                                        \
    (((unsigned long)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

// Define a simple class for testing
class MyClass {
  public:
    int data;
    double data2;
};

// Define a struct for testing
struct MyStruct {
    float value;
    char value2;
    MyClass *value3;
};

// Define a union for testing
union MyUnion {
    MyStruct *structValue;
    float floatValue;
};

// Define an enum for testing
enum class MyEnum { VALUE1, VALUE2, VALUE3 };

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
DEFINE_TEST_G(Test5, BumpDown) {
    BumpDown<1024> bumpAllocator;

    // Test with a class
    MyClass *myClassInstance = bumpAllocator.alloc<MyClass>(1);
    TEST_MESSAGE(myClassInstance != nullptr, "Allocation failed for MyClass!");
    myClassInstance->data = 42;
    TEST_MESSAGE(myClassInstance->data == 42, "incorrect data");

    // Test with a struct
    MyStruct *myStructInstance = bumpAllocator.alloc<MyStruct>(1);
    TEST_MESSAGE(myStructInstance != nullptr,
                 "Allocation failed for MyStruct!");
    myStructInstance->value = 3.14f;
    myStructInstance->value2 = 'a';
    myStructInstance->value3 = myClassInstance;
    TEST_MESSAGE(myStructInstance->value == 3.14f, "incorrect data");
    TEST_MESSAGE(myStructInstance->value2 == 'a', "incorrect data");
    TEST_MESSAGE(myStructInstance->value3 == myClassInstance, "incorrect data");

    // Test with a union
    MyUnion *myUnionInstance = bumpAllocator.alloc<MyUnion>(1);
    TEST_MESSAGE(myUnionInstance != nullptr, "Allocation failed for MyUnion!");
    myUnionInstance->floatValue = 42;
    TEST_MESSAGE(myUnionInstance->floatValue == 42, "incorrect data");
    myUnionInstance->structValue = myStructInstance;
    TEST_MESSAGE(myUnionInstance->structValue == myStructInstance,
                 "incorrect data");

    // Test with an enum (not an object, just checking compilation)
    MyEnum *myEnumInstance = bumpAllocator.alloc<MyEnum>(1);
    TEST_MESSAGE(myEnumInstance != nullptr, "Allocation failed for MyEnum!");

    // Test if the first allocation is still valid
    TEST_MESSAGE(myClassInstance->data == 42, "incorrect data");

    // Check the total number of allocations
    TEST_MESSAGE(bumpAllocator.get_num_allocations() == 4,
                 "wrong number of allocations");
}

DEFINE_TEST_G(Test6, BumpDown) {
    BumpDown<1024> bumpAllocator;

    // Test with a class
    MyClass *myClassInstance = bumpAllocator.alloc<MyClass>(1);
    TEST_MESSAGE(is_aligned(myClassInstance, alignof(MyClass)),
                 "Alignment is incorrect for this type");

    // Test with a struct
    MyStruct *myStructInstance = bumpAllocator.alloc<MyStruct>(1);
    TEST_MESSAGE(is_aligned(myStructInstance, alignof(MyStruct)),
                 "Alignment is incorrect for this type");

    // Test with a union
    MyUnion *myUnionInstance = bumpAllocator.alloc<MyUnion>(1);
    TEST_MESSAGE(is_aligned(myUnionInstance, alignof(MyUnion)),
                 "Alignment is incorrect for this type");

    // Test with an enum (not an object, just checking compilation)
    MyEnum *myEnumInstance = bumpAllocator.alloc<MyEnum>(1);
    TEST_MESSAGE(is_aligned(myEnumInstance, alignof(MyEnum)),
                 "Alignment is incorrect for this type");
}

DEFINE_TEST_G(Test7, BumpDown) {
    BumpDown<sizeof(int) * 15> b;

    for (int i = 0; i < 5; i++) {
        b.force_dealloc();
        do {
            b.alloc<char>(1);
            b.alloc<short>(1);
            b.alloc<char>(1);
        } while (b.alloc<int>(1));
        TEST_MESSAGE(b.get_num_allocations() == 20,
                     "incorrect allocation number");
    }
    for (int i = 19; i >= 0; i--) {
        b.dealloc();
        TEST_MESSAGE(b.get_num_allocations() == i,
                     "incorrect allocation number");
    }
}

DEFINE_TEST_G(Test8, BumpDown) {
    BumpDown<sizeof(int) * 3 + sizeof(float) * 3 + sizeof(MyClass) * 3> b;

    TEST_MESSAGE(b.alloc<int>(3) != nullptr, "failed to allocate");
    TEST_MESSAGE(b.alloc<float>(3) != nullptr, "failed to allocate");
    TEST_MESSAGE(b.alloc<MyClass>(3) != nullptr, "failed to allocate");
    TEST_MESSAGE(b.alloc<char>(1) == nullptr, "should have failed to allocate");
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

DEFINE_TEST_G(Test5, BumpUp) {
    BumpUp<1024> bumpAllocator;

    // Test with a class
    MyClass *myClassInstance = bumpAllocator.alloc<MyClass>(1);
    TEST_MESSAGE(myClassInstance != nullptr, "Allocation failed for MyClass!");
    myClassInstance->data = 42;
    TEST_MESSAGE(myClassInstance->data == 42, "incorrect data");

    // Test with a struct
    MyStruct *myStructInstance = bumpAllocator.alloc<MyStruct>(1);
    TEST_MESSAGE(myStructInstance != nullptr,
                 "Allocation failed for MyStruct!");
    myStructInstance->value = 3.14f;
    myStructInstance->value2 = 'a';
    myStructInstance->value3 = myClassInstance;
    TEST_MESSAGE(myStructInstance->value == 3.14f, "incorrect data");
    TEST_MESSAGE(myStructInstance->value2 == 'a', "incorrect data");
    TEST_MESSAGE(myStructInstance->value3 == myClassInstance, "incorrect data");

    // Test with a union
    MyUnion *myUnionInstance = bumpAllocator.alloc<MyUnion>(1);
    TEST_MESSAGE(myUnionInstance != nullptr, "Allocation failed for MyUnion!");
    myUnionInstance->floatValue = 42;
    TEST_MESSAGE(myUnionInstance->floatValue == 42, "incorrect data");
    myUnionInstance->structValue = myStructInstance;
    TEST_MESSAGE(myUnionInstance->structValue == myStructInstance,
                 "incorrect data");

    // Test with an enum (not an object, just checking compilation)
    MyEnum *myEnumInstance = bumpAllocator.alloc<MyEnum>(1);
    TEST_MESSAGE(myEnumInstance != nullptr, "Allocation failed for MyEnum!");

    // Test if the first allocation is still valid
    TEST_MESSAGE(myClassInstance->data == 42, "incorrect data");

    // Check the total number of allocations
    TEST_MESSAGE(bumpAllocator.get_num_allocations() == 4,
                 "wrong number of allocations");
}

DEFINE_TEST_G(Test6, BumpUp) {
    BumpUp<1024> bumpAllocator;

    // Test with a class
    MyClass *myClassInstance = bumpAllocator.alloc<MyClass>(1);
    TEST_MESSAGE(is_aligned(myClassInstance, alignof(MyClass)),
                 "Alignment is incorrect for this type");

    // Test with a struct
    MyStruct *myStructInstance = bumpAllocator.alloc<MyStruct>(1);
    TEST_MESSAGE(is_aligned(myStructInstance, alignof(MyStruct)),
                 "Alignment is incorrect for this type");

    // Test with a union
    MyUnion *myUnionInstance = bumpAllocator.alloc<MyUnion>(1);
    TEST_MESSAGE(is_aligned(myUnionInstance, alignof(MyUnion)),
                 "Alignment is incorrect for this type");

    // Test with an enum (not an object, just checking compilation)
    MyEnum *myEnumInstance = bumpAllocator.alloc<MyEnum>(1);
    TEST_MESSAGE(is_aligned(myEnumInstance, alignof(MyEnum)),
                 "Alignment is incorrect for this type");
}

DEFINE_TEST_G(Test7, BumpUp) {
    BumpUp<sizeof(int) * 15> b;

    for (int i = 0; i < 5; i++) {
        b.force_dealloc();
        do {
            b.alloc<char>(1);
            b.alloc<short>(1);
            b.alloc<char>(1);
        } while (b.alloc<int>(1));
        TEST_MESSAGE(b.get_num_allocations() == 20,
                     "incorrect allocation number");
    }
    for (int i = 19; i >= 0; i--) {
        b.dealloc();
        TEST_MESSAGE(b.get_num_allocations() == i,
                     "incorrect allocation number");
    }
}

DEFINE_TEST_G(Test8, BumpUp) {
    BumpUp<sizeof(int) * 3 + sizeof(float) * 3 + sizeof(MyClass) * 3> b;

    TEST_MESSAGE(b.alloc<int>(3) != nullptr, "failed to allocate");
    TEST_MESSAGE(b.alloc<float>(3) != nullptr, "failed to allocate");
    TEST_MESSAGE(b.alloc<MyClass>(3) != nullptr, "failed to allocate");
    TEST_MESSAGE(b.alloc<char>(1) == nullptr, "should have failed to allocate");
}

int main() {
    bool pass = true;

    for (auto group : groups) {
        pass &= TestFixture::ExecuteTestGroup(group, TestFixture::Verbose);
    }

    return pass ? 0 : 1;
}
