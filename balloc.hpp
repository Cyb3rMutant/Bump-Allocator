#include <cstdint>

typedef char byte;

template <unsigned S> class BumpUp {
  public:
    BumpUp() {
        start = new byte[S];
        ptr = start;
        end = start + S;
        num_allocations = 0;
    }

    template <class T> T *alloc(unsigned n) {
        unsigned size = sizeof(T) * n;

        byte *aligned = reinterpret_cast<byte *>(
            (reinterpret_cast<uintptr_t>(ptr) - 1u + alignof(T)) & -alignof(T));

        byte *new_ptr = aligned + size;

        if (new_ptr > end) {
            return nullptr;
        }

        ptr = new_ptr;

        num_allocations++;
        return (T *)aligned;
    }

    int get_num_allocations() { return num_allocations; }

    void dealloc() {
        if (--num_allocations == 0)
            force_dealloc();
    }

    void force_dealloc() { ptr = start; }

    ~BumpUp() { delete[] start; }

  private:
    byte *start;
    byte *ptr;
    byte *end;

    int num_allocations;
};
