#ifndef ALLOC_H
#define ALLOC_H

// Function to initialize the memory manager
// Must allocate a 4KB page via mmap and initialize data structures. 
// Returns 0 on success, non-zero on failure. 
int init_alloc();

// Function to cleanup the memory manager state.
// Must return the memory mapped page back to the OS. 
// Returns 0 on success, non-zero on failure. 
int cleanup();

// Function to allocate memory of a given size (multiple of 8 bytes). 
// Returns a char* pointer to the buffer on success. 
// Returns NULL on failure (e.g., size not multiple of 8, insufficient space). 
char *alloc(int size);

// Function to deallocate a previously allocated memory chunk. 
// ptr must point to a chunk previously returned by alloc().
void dealloc(char *ptr);

#endif