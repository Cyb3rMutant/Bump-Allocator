typedef char byte;

template <unsigned S> class BumpDown {
  public:
    BumpDown() {
        memory = new byte[S];
        esp = memory + S;
        num_allocations = 0;
    }

    template <class T> T *alloc(unsigned n) {
        unsigned size = sizeof(T) * n;

        unsigned long remainder = 0;
        if ((remainder = ((unsigned long)esp % alignof(T)))) {
            esp -= remainder;
        }

        if ((esp - size) < memory) {
            esp += remainder;
            return nullptr;
        }

        esp -= size;

        num_allocations++;
        return (T *)esp;
    }

    void dealloc() {
        if (--num_allocations == 0) {
            delete[] memory;
            memory = new byte[S];
            esp = memory + S;
        }
    }

    int get_num_allocations() { return num_allocations; }

  private:
    byte *memory;
    byte *esp;
    int num_allocations;
};
