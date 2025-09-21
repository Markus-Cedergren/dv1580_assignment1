
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
    

}

struct metadata* findFreeBlock(struct metadata* block, size_t size){
    struct metadata* currentBlock = block; //the block to work from

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
    struct metadata* current_block = (struct metadata*) memory_pool;
    struct metadata* freeBlock = findFreeBlock(current_block, size);
    if (freeBlock == NULL)
        return NULL;

    if(freeBlock->size_of_block >= size+sizeof(struct metadata)+1){ //split the free block
        struct metadata* oldNextBlock = freeBlock->pos_next_block;
        size_t storeSize = freeBlock->size_of_block;
        freeBlock->free = 0;
        freeBlock->size_of_block = size;
        void* userSpace = (void*)(freeBlock+1);
        char* nextBlockAddress = (char*)(freeBlock+1)+size;
        struct metadata* newBlock = (struct metadata*) nextBlockAddress;
        freeBlock->pos_next_block = newBlock;
        newBlock->free = 1;
        newBlock->pos_next_block = oldNextBlock;
        newBlock->size_of_block = storeSize-size-sizeof(struct metadata);
        newBlock->id = getUniqueId();

        return userSpace;
    }
    else{ //Use the entire free block
        freeBlock->free = 0;
        void* userSpace = (void*)(freeBlock+1);
        return userSpace;
    }

    return NULL;
    
}



void print_all_block(){
    struct metadata* currentBlock = (struct metadata*) memory_pool;

    while (currentBlock != NULL){
        int blockID = currentBlock->id;
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
    struct metadata* blockToResize = ((struct metadata*) block)-1;

    if(blockToResize->size_of_block > size){ //shrink (split and create new block)

        size_t originalSize = blockToResize->size_of_block;
        struct metadata* originalNextBlock = blockToResize->pos_next_block;

        blockToResize->size_of_block = size;
        char* fillBlockAdress = (char*)(blockToResize+1)+size;
        struct metadata* fillBlock = (struct metadata*) fillBlockAdress;
        blockToResize->pos_next_block = fillBlock;
        fillBlock->free = 1;
        fillBlock->pos_next_block = originalNextBlock;
        fillBlock->size_of_block = originalSize-size-sizeof(struct metadata);
        fillBlock->id = getUniqueId();
        return blockToResize + 1;
    }

    else{ //grow, allocate more space. 

        //Kolla om föregåenge block är stort nog, eller om nästa block är stort nog.
        // Om inte, skapa ett nytt block, kopiera över. 

        struct metadata* nextBlock = blockToResize->pos_next_block;

        if( //use the next block
            nextBlock != NULL && 
            nextBlock->free && 
            nextBlock->size_of_block + blockToResize->size_of_block + sizeof(struct metadata) >= size){
            
                if(blockToResize->size_of_block + nextBlock->size_of_block + sizeof(struct metadata) >= size + sizeof(struct metadata) + 1){ // merge two blocks, create one additionall block to fill grap

                    size_t blockToResizeSize = blockToResize->size_of_block;
                    size_t nextBlockSize = nextBlock->size_of_block;
                    struct metadata* nextBlockNext = nextBlock->pos_next_block;

                    blockToResize->size_of_block = size;
                    size_t gapSpace = blockToResizeSize + nextBlockSize - size - sizeof(struct metadata);

                    char* fillBlockAdress = (char*)(blockToResize+1)+blockToResize->size_of_block;
                    struct metadata* fillBlock = (struct metadata*)fillBlockAdress;
                    fillBlock->free = 1;
                    fillBlock->size_of_block = gapSpace;

                    blockToResize->pos_next_block = fillBlock;
                    fillBlock->pos_next_block = nextBlockNext;
                    fillBlock->id = getUniqueId();

                    return blockToResize+1;

                }else{ //If no gap i created, only merge blocks

                    blockToResize->size_of_block = blockToResize->size_of_block + nextBlock->size_of_block + sizeof(struct metadata);
                    blockToResize->pos_next_block = nextBlock->pos_next_block;
                    return blockToResize+1;

                }


        }
        else{//use a new block via mem_alloc

            void* newBlock = mem_alloc(size);
            size_t byteToCopy = blockToResize->size_of_block;
            memcpy(newBlock, block,byteToCopy);
            blockToResize->free=1;
            return newBlock;

        }
        


    }    
}


void mem_deinit(){
    /*
Frees up the memory pool that was initially allocated by the mem_init function, ensuring that all allocated memory is returned to the system.
*/

free(memory_pool);

}



