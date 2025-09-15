
#include "memory_manager.h"
#include <stdlib.h>
#include <stdio.h>

char * memory_pool = NULL; //global memory pool

int counter = 1;
/*
Initializes the memory manager with a specified size of memory pool. The memory pool could be any data structure, for instance, a large array or a similar contuguous block of memory. (You do not have to interact directly with the hardware or the operating system’s memory management functions).
*/
void mem_init(size_t size){
    printf("Initializing the memory %ld \n", size);

    if ((memory_pool = malloc(size)) == NULL){
        return;
    }

    struct metadata *init_block = (struct metadata*) memory_pool;
    //Memory pool innehåller addressen till första "sloten" i minnesblocket som allokerats.
    //init_block innehåller addressen till första "sloten" i minnesblocket som allokerats.
    
    init_block->free = 1;
    init_block->size_of_block = size - sizeof(struct metadata);
    init_block->pos_next_block = NULL;


    init_block->id = counter;
    counter++;
    printf("Adding the start struct to the memory-pool at address %p, with the size of %ld \n", (void*)init_block,size - sizeof(struct metadata));

}



/* 
Allocates a block of memory of the specified size. Find a suitable block in the pool, mark it as allocated, and return the pointer to the start of the allocated block.
*/
void* mem_alloc(size_t size){

    // printf("Adding a new block to the memory \n");
    struct metadata* current_block = (struct metadata*) memory_pool; //pointer to the start of the allocated memory. casted as a struct

    void* userData = NULL;
    while(current_block != NULL){
        

        if (current_block->free == 1 && current_block->size_of_block >= size){
            current_block->free = 0;
            userData = (void*)(current_block+1); //Set the user data to the first address after the meta data. 

            char* next_block_address = (char*)(current_block+1)+size; //get the address of the next metadata struct. This is after the userData
            struct metadata* new_block = (struct metadata*) next_block_address; //add the new block to the allocated memory
            current_block->pos_next_block = new_block; //add the new block as the "next block" of the current block
            
            new_block->id = counter; 
            counter++;
            new_block->free = 1; //Set the new block as free

            new_block->size_of_block = current_block->size_of_block - size - sizeof(struct metadata); //Set the new block size
            current_block->size_of_block = size; //Set the size of the current block to size
            return userData; //Return a pointer to the userData space. 
    }


    current_block = current_block->pos_next_block; //get next block

    }

    return NULL;
}



void print_all_block(){
    struct metadata* currentBlock = (struct metadata*) memory_pool;

    while (currentBlock != NULL){
        int blockID = currentBlock->id;

        printf("Current Block is: %d  and have the size: %ld, and have taken-status: %d\n", blockID, currentBlock->size_of_block, currentBlock->free);

        currentBlock = currentBlock->pos_next_block;
    }

    
}




void mem_free(void* block){
/* 
Frees the specified block of memory. For allocation and deallocation, you need a way to track which parts of the memory pool are free and which are allocated.
*/

    struct metadata* blockToFree = ((struct metadata*) block)-1; //Need to do -1 since the user have the adress to the userSpace. Want to modify the struct
    blockToFree->free = 1;

}



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



