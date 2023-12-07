#pragma once

/**
 * @file r_balloc.hpp
 * @brief Defines the BumpDown class for efficient bump-pointer memory
 * allocation.
 */

#include <cstddef> // For size_t
#include <cstdint> // For uintptr_t

using std::byte;

/**
 * @class BumpDown
 * @brief A simple bump-pointer allocator for memory allocation and
 * deallocation.
 * @tparam S The size of the memory buffer to be allocated.
 */
template <size_t S> class BumpDown {
  public:
    /**
     * @brief Constructor for the BumpDown class.
     * Initializes the memory buffer and the bump pointer.
     */
    BumpDown() {
        start = new byte[S];
        end = start + S;
        ptr = end;
        num_allocations = 0;
    }

    /**
     * @brief Allocates memory for an array of elements of type T.
     *
     * @tparam T The type of elements to allocate.
     * @param n Number of elements to allocate space for.
     * @returns A pointer to the allocated memory, or nullptr if allocation
     * fails.
     *
     * This function allocates memory for an array of elements of type T. The
     * allocation is aligned to the specified alignment for type T. The function
     * keeps track of the number of allocations made using a counter for
     * freeing.
     *
     * @note The alignment is determined using the alignof(T) to ensure proper
     * alignment for the allocated memory.
     */
    template <class T> T *alloc(size_t n) {
        // Calculate the size needed for the allocation
        size_t size = sizeof(T) * n;

        // Calculate the new pointer after the allocation
        byte *new_ptr = ptr - size;

        // Calculate the aligned memory location
        new_ptr = reinterpret_cast<byte *>(
            (reinterpret_cast<uintptr_t>(new_ptr)) & -alignof(T));

        // Check if the allocation exceeds the available memory
        if (new_ptr < start) {
            return nullptr;
        }

        // Update the current pointer to the new position
        ptr = new_ptr;

        // Increment the global allocation counter
        num_allocations++;

        // Return the aligned pointer to the allocated memory
        return (T *)ptr;
    }

    /**
     * @brief Gets the total number of allocations made using this allocator.
     * @return The number of allocations.
     */
    int get_num_allocations() { return num_allocations; }

    /**
     * @brief Deallocates memory for objects of type T.
     * If the number of allocations becomes zero, forces deallocation of all
     * memory.
     */
    void dealloc() {
        if (--num_allocations == 0)
            force_dealloc();
    }

    /**
     * @brief Forces deallocation of all memory.
     */
    void force_dealloc() {
        ptr = end;
        num_allocations = 0;
    }

    /**
     * @brief Destructor for the BumpDown class.
     * Deallocates the memory buffer.
     */
    ~BumpDown() { delete[] start; }

  private:
    byte *start;         ///< Start of the allocated memory buffer.
    byte *ptr;           ///< Current bump pointer position.
    byte *end;           ///< End of the allocated memory buffer.
    int num_allocations; ///< Number of active allocations.
};
