#include <stdio.h>
#include <stdlib.h>

// Define block sizes
#define NUM_BLOCK_SIZES 3
#define BLOCK_SIZES {50, 100, 200}

// Define structure for a memory block
typedef struct Block {
    size_t size;            // Size of the block
    struct Block* next;     // Pointer to the next block
} Block;

// Free lists for different block sizes
Block* free_lists[NUM_BLOCK_SIZES] = {NULL};

// Initialize free lists
void initialize_free_lists() {
    for (int i = 0; i < NUM_BLOCK_SIZES; i++) {
        free_lists[i] = NULL;
    }
}

// Find the index of the appropriate free list for a given size
int find_list_index(size_t size) {
    size_t sizes[] = BLOCK_SIZES;
    for (int i = 0; i < NUM_BLOCK_SIZES; i++) {
        if (size <= sizes[i]) {
            return i;
        }
    }
    return -1;
}

// Allocate memory
void* quick_fit_malloc(size_t size) {
    int index = find_list_index(size);  // Get the correct list index
    if (index == -1) {
        printf("Error: Requested size %zu does not match any predefined block size.\n", size);
        return NULL;
    }
    // If a free block exists in the free list, allocate it
    if (free_lists[index] != NULL) {
        Block* block = free_lists[index];
        free_lists[index] = block->next;
        return (void*)(block + 1);
    }
    // Otherwise, allocate new memory from the system
    size_t total_size = sizeof(Block) + size;
    Block* block = (Block*)malloc(total_size);
    if (!block) {
        printf("Error: Memory allocation failed.\n");
        return NULL;
    }
    block->size = size;
    return (void*)(block + 1);
}

// Free memory
void quick_fit_free(void* ptr) {
    if (!ptr) return;

    Block* block = (Block*)ptr - 1;  // Get the block header
    int index = find_list_index(block->size);
    if (index == -1) {
        printf("Error: Block size %zu does not match any predefined block size.\n", block->size);
        free(block);
        return;
    }

    // Add the block back to the free list
    block->next = free_lists[index];
    free_lists[index] = block;
}

// Print the state of the free lists
void print_free_lists() {
    size_t sizes[] = BLOCK_SIZES;
    for (int i = 0; i < NUM_BLOCK_SIZES; i++) {
        printf("Free list for %zu KB blocks: ", sizes[i]);
        Block* current = free_lists[i];
        while (current) {
            printf("[%zu KB] -> ", current->size);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Main function to demonstrate the algorithm
int main() {
    initialize_free_lists();  // Initialize the free lists

    printf("Memory Allocation System Initialized.\n");

    // Simulate memory allocations
    void* block1 = quick_fit_malloc(50);   // Allocate 50 KB block
    void* block2 = quick_fit_malloc(100);  // Allocate 100 KB block
    void* block3 = quick_fit_malloc(200);  // Allocate 200 KB block

    // Display memory state after allocations
    printf("\nAfter allocations:\n");
    print_free_lists();

    // Simulate deallocations
    quick_fit_free(block1);  // Free the 50 KB block
    quick_fit_free(block2);  // Free the 100 KB block
    quick_fit_free(block3);  // Free the 200 KB block

    // Display memory state after deallocations
    printf("\nAfter deallocations:\n");
    print_free_lists();

    return 0;
}
