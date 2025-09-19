
#include "memory_manager.h"
#include <stdlib.h>
#include <stdio.h>

char * memory_pool = NULL; //global memory pool

int counter = 1;


int getUniqueId(){
    int retval = counter;
    counter++;

    return retval;
}

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

    init_block->id = getUniqueId();
    
    printf("Adding the start struct to the memory-pool at address %p, with the size of %ld \n", (void*)init_block,size - sizeof(struct metadata));

}

struct metadata* findNextFreeBlock(struct metadata* block, size_t size){
    struct metadata* currentBlock = block->pos_next_block; //the block to work from

    while (currentBlock != NULL){
        if(currentBlock->free == 1 && currentBlock->size_of_block>= size){
            return currentBlock;

        }
        else{
            currentBlock = currentBlock->pos_next_block;
        }
    }
    return NULL;
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
    //if the new size is smaller then the current block: Shrink the memory, add a new block to fill the gap
    

    struct metadata* blockToResize = ((struct metadata*) block)-1;
    size_t original_size = blockToResize->size_of_block;

    if(original_size>= size+sizeof(struct metadata)){
        struct metadata* oldNextBlock = blockToResize->pos_next_block; //get the "next" block of the blocket in question. Needed to complete the chain of references
        blockToResize->size_of_block = size; //Set the size of the block in question to the new specified size
        char* next_block_address = (char*)(blockToResize+1)+size; //get the address of the next metadata struct. This is after the userData
        struct metadata* new_block = (struct metadata*) next_block_address; //add a new block to in the gap-space created by the resizing
        blockToResize->pos_next_block = new_block; //set the "next block" of the block in question to the new block. 
        new_block->size_of_block = original_size-size-sizeof(struct metadata); //Set the size of the new
        new_block->free = 1; //Set the new block as free
        new_block->pos_next_block = oldNextBlock; //set the next-block of the new block to the original next block. Completes the chain of "next-block"
        new_block->id = getUniqueId();

        //If the new size is larger than the current size

        return blockToResize+1;
    }else{
        
    }


    
}


void mem_deinit(){
    /*
Frees up the memory pool that was initially allocated by the mem_init function, ensuring that all allocated memory is returned to the system.
*/

free(memory_pool);

}



