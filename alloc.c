#include "alloc.h"     
#include <stdio.h>    
#include <sys/mman.h> 
#include <stdlib.h> 
#define PAGE_SIZE 4096  //Definition of memory page size (4KB)
static char *memory_page = NULL; // Static pointer to the start of the 4KB memory page, initialized to NULL
// Defines the MemoryBLOCK struct to represent memory chunks within the 4KB page (free or allocated), including address, size, and status in a doubly linked list
typedef struct MemoryBlock {
    char *address; 
    int size;      
    int is_free;   
    struct MemoryBlock *next; 
    struct MemoryBlock *prev; 
} MemoryBlock;
// Static head of the linked list tracking all memory blocks within the 4KB page, initialized to NULL
static MemoryBlock *head_block_list= NULL;
// Initializes the memory manager
int init_alloc(){
    //If it is already initialized, perform cleanup
    if(memory_page != NULL){
        if (cleanup() != 0) {
            fprintf(stderr, "init_alloc: failed to cleanup previous memory state. \n");
            return -1;
         }
    }
    // via mmap request a 4KB page
    memory_page = (char *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // to check if mmap failed
     if (memory_page == MAP_FAILED) {
        perror("init_alloc: mmap failed");
        return -1 ;  
     }
    // Metadata initialization: page starts with 4KB as one free block
    head_block_list = (MemoryBlock *)malloc (sizeof(MemoryBlock));
    // to check if metadata malloc failure
    if (head_block_list == NULL) {
        perror("init_alloc: Failed to allocate head_block_list metadata");
        munmap(memory_page, PAGE_SIZE); 
        memory_page = NULL; 
        return -1 ;   
     }
    //fill(populate) the first MemoryBlock
    head_block_list->address = memory_page; 
    head_block_list->size = PAGE_SIZE;     
    head_block_list->is_free = 1;         
    head_block_list-> next = NULL;           
    head_block_list->prev =NULL ;
    // print success  
     printf("Memory manager initialized successfully. 4KB page allocated at %p \n", memory_page);
    return 0 ;  
}
// Cleans up the memory manager stste
int cleanup(){
    // to check if memory manager is initialized
    if (memory_page == NULL) {
        printf("Cleanup: Memory manager not initialized, nothing to cleanup.\n");
        return 0 ;
     }
    // unmap the 4kb memory page from OS
    if(munmap(memory_page, PAGE_SIZE ) != 0){
        perror("cleanup: munmap failed");
        return -1 ;
    }
    memory_page = NULL;
    // Free all MemoryBlock metadata structs
    MemoryBlock *current = head_block_list;
    while (current != NULL) {
        MemoryBlock *next_block =current->next ; 
        free(current) ; 
        current = next_block; 
     }
    head_block_list = NULL; 
    //print success
    printf("Memory manager cleaned up successfully.\n" );
    return 0; 
}
// allocates a memory chunk
char *alloc(int size) {
    // check that the requsted size is correct
    if (size <= 0 || size % 8 != 0 ) {
        fprintf(stderr, "alloc: Invalid size requested (%d). Must be positive and a multiple of 8. \n", size );
        return NULL;
     }
    // find suitable free block (First Fit)
    MemoryBlock *current = head_block_list;
    while (current != NULL) {
        if (current->is_free && current->size >= size) { // found block
            // if block is significantly larger, split
            int remaining_size = current->size - size;
            if (remaining_size >= (sizeof(MemoryBlock) + 8)) {
                // create new metadata for remaining free part  
                MemoryBlock *new_free_block =(MemoryBlock *)malloc(sizeof(MemoryBlock)) ;
                if (new_free_block == NULL) {
                    perror("alloc: Failed to allocate metadata for new free block" );
                    return NULL ;  
                 }
                //setup new free block and update list pointers
                new_free_block->address = current->address + size;  
                new_free_block->size = remaining_size;
                new_free_block->is_free = 1;
                new_free_block->next =current->next;
                new_free_block->prev = current;
                if (current->next != NULL) {
                    current->next->prev =new_free_block;
                 }
                current->next = new_free_block ;
                current->size = size ; //Update allocated block size         
             }
            // Mark allocated and return address 
            current->is_free = 0; 
            printf("Allocated %d bytes at %p\n", size, current-> address);
            return current-> address;
        }
        // the next block
        current = current ->next;  
     }
    // Print no free block
    fprintf (stderr, "alloc: No suitable free block found for size %d. \n", size);
    return NULL ;  
}
// Dealloctes a memory chunk
void dealloc(char *ptr){
    // to validate pointer
    if (ptr == NULL) {
        fprintf(stderr, "dealloc: Attempted to deallocate a NULL pointer. \n" );
        return;
    }
    // to check pointer range
    if (ptr < memory_page || ptr>= ( memory_page + PAGE_SIZE) ) {
        fprintf(stderr, "dealloc: Pointer %p is outside managed memory range.\n",  ptr) ;
        return ;
     }
     // find corresponding MemoryBlock
    MemoryBlock *current = head_block_list ;
    MemoryBlock *block_to_deallocate = NULL;
    while (current != NULL) {
        if (current->address == ptr) {
            block_to_deallocate = current;
            break;
        }
        current = current->next ;
     }
     // to check if bloc found, and allocated
    if( block_to_deallocate == NULL)  {
        fprintf (stderr, "dealloc: Pointer %p does not correspond to an allocated block.\n ", ptr );
        return;
      }
    if (block_to_deallocate->is_free ) {
        fprintf(stderr, "dealloc: Pointer %p already points to a free block. \n" , ptr);
        return;
     }
    // mark block free
    block_to_deallocate->is_free = 1;
    printf ( " Deallocated %d bytes at %p \n" , block_to_deallocate-> size, block_to_deallocate->address);
    // merge with previous block
    if ( block_to_deallocate->prev != NULL && block_to_deallocate ->prev->is_free){
        block_to_deallocate->prev->size += block_to_deallocate-> size;
        block_to_deallocate->prev->next = block_to_deallocate->next;
        if(block_to_deallocate->next !=NULL){
            block_to_deallocate->next->prev = block_to_deallocate->prev;
         } 
        free( block_to_deallocate) ;
        block_to_deallocate =block_to_deallocate->prev ;
    }
    // merge with next block
    if (block_to_deallocate->next != NULL && block_to_deallocate->next->is_free ) {
        MemoryBlock *next_block =block_to_deallocate->next;
        block_to_deallocate->size += next_block ->size;
        block_to_deallocate->next =next_block->next;
        if (next_block->next != NULL ){
            next_block->next -> prev = block_to_deallocate ;
         }
        // free next_block metadata.
        free (next_block) ;
     }
 }