
#include "memory_manager.h"
#include <stdlib.h>
#include <stdio.h>

char * memory_pool = NULL; //global memory pool

struct metadata{
    size_t size_of_block;
    int free;
    struct metadata* pos_next_block;
};





/*
Initializes the memory manager with a specified size of memory pool. The memory pool could be any data structure, for instance, a large array or a similar contuguous block of memory. (You do not have to interact directly with the hardware or the operating system’s memory management functions).
*/
void mem_init(size_t size){
    printf("Initializing the memory %ld \n", size);

    if ((memory_pool = malloc(size)) == NULL){
        return;
    }

    struct metadata *init_block = (struct metadata*) memory_pool;
    printf("Adding the start struct to the memory-pool at address %s \n", init_block);
    init_block->free = 1;
    init_block->size_of_block = size - sizeof(struct metadata);
    init_block->pos_next_block = NULL;
    printf("With the size of %ld \n", size - sizeof(struct metadata));


    




}



/* 
Allocates a block of memory of the specified size. Find a suitable block in the pool, mark it as allocated, and return the pointer to the start of the allocated block.
*/
void* mem_alloc(size_t size){

    struct metadata* current_block = (struct metadata*) memory_pool;
    void* user_data = NULL;
    while(current_block != NULL){
        

        if (current_block->free == 1 && current_block->size_of_block >= size){
            current_block->free = 0;
            user_data = (void*)(current_block+1);

            char* next_block_address = (char*)(current_block+1)+size;
            struct metadata* new_block = (struct metadata*) next_block_address;
            new_block->free = 1;
            new_block->size_of_block = current_block->size_of_block - size - sizeof(struct metadata);
            return user_data;
    }


    current_block = current_block->pos_next_block;

    }

    return NULL;
}


void mem_free(void* block){
/* 
Frees the specified block of memory. For allocation and deallocation, you need a way to track which parts of the memory pool are free and which are allocated.
*/}



void* mem_resize(void* block, size_t size){
/*
Changes the size of the memory block, possibly moving it.

*/

}



void mem_deinit(){
    /*
Frees up the memory pool that was initially allocated by the mem_init function, ensuring that all allocated memory is returned to the system.
*/

free(memory_pool);

}



