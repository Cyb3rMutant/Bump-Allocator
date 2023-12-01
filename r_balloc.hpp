#include <cstdint>

typedef char byte;

template <unsigned S> class BumpDown {
  public:
    BumpDown() {
        start = new byte[S];
        end = start + S;
        ptr = end;
        num_allocations = 0;
    }

    template <class T> T *alloc(unsigned n) {
        unsigned size = sizeof(T) * n;

        byte *new_ptr = ptr - size;

        new_ptr = reinterpret_cast<byte *>(
            (reinterpret_cast<uintptr_t>(new_ptr)) & -alignof(T));

        if (new_ptr < start) {
            return nullptr;
        }

        ptr = new_ptr;

        num_allocations++;
        return (T *)ptr;
    }

    int get_num_allocations() { return num_allocations; }

    void dealloc() {
        if (--num_allocations == 0)
            force_dealloc();
    }

    void force_dealloc() { ptr = end; }

    ~BumpDown() { delete[] start; }

  private:
    byte *start;
    byte *ptr;
    byte *end;

    int num_allocations;
};
