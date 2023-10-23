#include <cstdlib>
#include <cstring>
class Heap {
  public:
    Heap() {
        memory = malloc(9999999);
        esp = &memory;
        num_allocations = 0;
    }

    template <class T> auto alloc(T obj) -> T {
        unsigned obj_size = sizeof(obj);
        if (*((int *)*esp + obj_size) > *((int *)memory + 9999999)) {
            return nullptr;
        }

        memcpy(*esp, &obj, obj_size);
        void *temp_esp = *esp;
        *esp = ((int *)*esp + obj_size);

        num_allocations++;
        return temp_esp;
    }
    void dealloc() {
        if (--num_allocations != 0) {
            free(memory);
        }
    }

  private:
    void *memory;
    void **esp;
    int num_allocations;
};
