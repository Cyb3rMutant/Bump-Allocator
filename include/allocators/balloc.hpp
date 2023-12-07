#pragma once

/**
 * @file balloc.hpp
 * @brief Defines the BumpUp class for efficient bump-pointer memory
 * allocation.
 */

#include <cstddef> // For size_t
#include <cstdint> // For uintptr_t

using std::byte;

/**
 * @class BumpUp
 * @brief A simple bump-pointer allocator for memory allocation and
 * deallocation.
 * @tparam S The size of the memory buffer to be allocated.
 */
template <size_t S> class BumpUp {
  public:
    /**
     * @brief Constructor for the BumpUp class.
     * Initializes the memory buffer and the bump pointer.
     */
    BumpUp() {
        start = new byte[S];
        ptr = start;
        end = start + S;
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

        // Calculate the alignment value for type T
        size_t alignment = alignof(T);

        // Calculate the aligned memory location
        byte *aligned = reinterpret_cast<byte *>(
            (reinterpret_cast<uintptr_t>(ptr) - 1u + alignment) & -alignment);

        // Calculate the new pointer after the allocation
        byte *new_ptr = aligned + size;

        // Check if the allocation exceeds the available memory
        if (new_ptr > end) {
            return nullptr;
        }

        // Update the current pointer to the new position
        ptr = new_ptr;

        // Increment the global allocation counter
        num_allocations++;

        // Return the aligned pointer to the allocated memory
        return reinterpret_cast<T *>(aligned);
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
        ptr = start;
        num_allocations = 0;
    }

    /**
     * @brief Destructor for the BumpUp class.
     * Deallocates the memory buffer.
     */
    ~BumpUp() { delete[] start; }

  private:
    byte *start;         ///< Start of the allocated memory buffer.
    byte *ptr;           ///< Current bump pointer position.
    byte *end;           ///< End of the allocated memory buffer.
    int num_allocations; ///< Number of active allocations.
};
