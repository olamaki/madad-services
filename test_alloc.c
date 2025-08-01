#include "alloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    //Init memory manager
    if (init_alloc() != 0 ) {
        fprintf(stderr, "Initialization failed. \n");
        return 1;
     }
    printf("Memory manager initialized.\n");

    // allocate first block
    char *ptr1 = alloc(32); // Request 32 bytes
    if (ptr1 == NULL) {
        fprintf(stderr , "Allocation of 32 bytes failed. \n");
        cleanup(); //Attempt cleanup before exit
        return 1;
      }
    printf ("Allocated 32 bytes at %p. \n", ptr1 );

    //allocate second block
    char *ptr2 = alloc(64); // Request 64 bytes
    if (ptr2 == NULL) {
        fprintf(stderr, "Allocation of 64 bytes failed.\n" );
        dealloc(ptr1); //Clean up previously allocated block
        cleanup() ;
        return 1;
    }
    printf(" Allocated 64 bytes at %p. \n", ptr2);

    //try allocating an invalid size
    char *ptr_invalid = alloc(10); // Should fail (not multiple of 8)
    if (ptr_invalid != NULL) {
        fprintf( stderr, " Allocated invalid size (10 bytes), which should fail.\n") ;
        dealloc( ptr1);
        dealloc(ptr2) ;
        cleanup();
        return 1 ;
     }
    printf(" Correctly refused invalid allocation of 10 bytes.\n") ;

    //Deallocate first block
    dealloc(ptr1);
    printf("Deallocated block at %p.\n", ptr1 );
    // Deallocate second block
    dealloc(ptr2) ;
    printf("Deallocated block at %p.\n", ptr2);
    //test merging by allocating a large block after deallocating two
    // Assuming (32 and 64) bytes were adjacent or became adjacent after deallocation
    char *ptr3 = alloc(96);// Sum of 32 + 64 (or close to it)
    if (ptr3 == NULL) {
        fprintf(stderr, "Allocation of 96 bytes after deallocations failed (possible merge issue) \n ");
        cleanup () ;
        return 1;
     }
    printf("Allocated 96 bytes at %p after deallocations \n", ptr3 );
    dealloc(ptr3); //Clean up for final cleanup
    // Cleanup memory manager
    if (cleanup() != 0 ) {
        fprintf (stderr, "Cleanup failed. \n" );
        return 1 ;
     }
    printf (" Memory manager cleaned up \n");
    printf ("\nAll basic tests passed successfully!\n") ;
    return 0 ;
}