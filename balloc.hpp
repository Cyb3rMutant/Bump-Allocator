typedef char byte;

template <unsigned S> class Bump {
  public:
    Bump() {
        memory = new byte[S];
        esp = memory;
        num_allocations = 0;
    }

    template <class T> T *alloc(unsigned n) {
        unsigned size = sizeof(T) * n;
        byte *temp = esp;

        if (unsigned long remainder = ((unsigned long)esp % alignof(T))) {
            temp += alignof(T) - remainder;
        }

        if ((temp + size) > (memory + S)) {
            return nullptr;
        }
        esp = temp + size;

        num_allocations++;
        return (T *)temp;
    }

    void dealloc() {
        // if (--num_allocations == 0) {
        delete[] memory;
        memory = new byte[S];
        esp = memory;
        // }
    }

    int get_num_allocations() { return num_allocations; }

  private:
    byte *memory;
    byte *esp;
    int num_allocations;
};
