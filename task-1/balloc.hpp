#include <cstdio>
#include <iostream>
#include <math.h>
#include <ostream>

typedef char byte;

template <unsigned S> class Heap {
  public:
    Heap() {
        memory = new byte[S];
        esp = memory;
        num_allocations = 0;
    }

    template <class T> T *alloc(unsigned n) {
        unsigned size = sizeof(T) * n;

        if ((esp + size) >= (memory + S)) {
            return nullptr;
        }
        void *temp_esp = esp;
        esp += size;

        if (auto remainder = ((unsigned long)esp % 4)) {
            remainder = 4 - remainder;
            esp += remainder;
        }
        num_allocations++;
        return (T *)temp_esp;
    }

    void dealloc() {
        if (--num_allocations == 0) {
            delete[] memory;
            memory = new byte[S];
            esp = memory;
        }
    }

    int get_num_allocations() { return num_allocations; }

  private:
    byte *memory;
    byte *esp;
    int num_allocations;
};
