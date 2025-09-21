
#include "memory_manager.h"
#include <stdlib.h>
#include <stdio.h>

char * memory_pool = NULL; //global memory pool

int counter = 1;
int numberOfBlocks = 0;

struct metadata *blocks = NULL;


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

    numberOfBlocks = 1;
    blocks = realloc(NULL, numberOfBlocks*sizeof(struct metadata));
    blocks[0].free=1;
    blocks[0].id = getUniqueId();
    blocks[0].offset = 0;
    blocks[0].size_of_block = size;
    
}


void shiftright(int fromIndex){ //shift block from specified position one index to the right.
    numberOfBlocks++;
    blocks = realloc(blocks, numberOfBlocks*sizeof(struct metadata));
    for(int index = numberOfBlocks-1; index >= fromIndex; index--){
        blocks[index] = blocks[index-1];
    }
}

void shiftleft(int fromIndex){
    for (int index = fromIndex; index<numberOfBlocks; index++){
        blocks[index] = blocks[index+1];
    }

    numberOfBlocks--;
    blocks = realloc(blocks, numberOfBlocks*sizeof(struct metadata));
}


void* mem_alloc(size_t size){
    for (int index = 0; index < numberOfBlocks; index++){

        if (blocks[index].free == 1 && blocks[index].size_of_block >= size){
            
            
            if(blocks[index].size_of_block == size){ //case where the free block is exactly the required size.
                blocks[index].free = 0;
                return memory_pool + blocks[index].offset;
            }

            else if(blocks[index].size_of_block > size){ //the case where there will be additionall memory left


                shiftright(index+1);
                blocks[index+1].id = getUniqueId();
                blocks[index+1].offset =  blocks[index].offset + size;
                blocks[index+1].size_of_block = blocks[index].size_of_block - size;
                blocks[index+1].free =1;

                blocks[index].free = 0;
                blocks[index].size_of_block = size;
                
                return blocks[index].offset + memory_pool;
            }
        }
    }
    return NULL;
}





void mem_free(void* block){

    size_t offsetToFind = (size_t)((char*)block - memory_pool);

    for(int index = 0; index < numberOfBlocks; index++){
        if(blocks[index].offset == offsetToFind){
            blocks[index].free = 1;
        
        if(index < numberOfBlocks-1 && index > 0 && blocks[index-1].free == 1 && blocks[index+1].free == 1){ //merge blocks.
            blocks[index-1].size_of_block = blocks[index].size_of_block + blocks[index+1].size_of_block + blocks[index-1].size_of_block;
            shiftleft(index);
            shiftleft(index);

        }

        }
    }
}


void* mem_resize(void* block, size_t size){

    size_t offsetToFind = (size_t)((char*) block-memory_pool);

    for(int index = 0; index < numberOfBlocks; index++){
        if(blocks[index].offset == offsetToFind){

            if(blocks[index].size_of_block >= size){ // shrink case.
                if(blocks[index].size_of_block >= size+1){ //need a new block to fill the space
                shiftright(index+1);             
                blocks[index+1].size_of_block = blocks[index].size_of_block - size;
                blocks[index+1].free = 1;
                blocks[index+1].id = getUniqueId();
                blocks[index+1].offset = blocks[index].offset+size;

                blocks[index].size_of_block = size;

                return block;
                }    
                else{ //handles edge case where resizing to the original size. 
                    return block;
                }
            }else{ //handles the case when block should grow. 

                if(blocks[index+1].free == 1 && blocks[index].size_of_block + blocks[index+1].size_of_block >= size){ //Grow into the free next block

                    if(blocks[index].size_of_block + blocks[index+1].size_of_block == size){ //perfect fit


                        blocks[index+1].free = 0;
                        
                        blocks[index].size_of_block = blocks[index].size_of_block + blocks[index+1].size_of_block;
                        shiftleft(index+1);
                        numberOfBlocks--;
                        return block;

                    }
                    else{ //create a new block to fill the size-gap after merging.
                        
                        blocks[index+1].size_of_block = blocks[index].size_of_block + blocks[index+1].size_of_block-size;
                        blocks[index+1].offset = blocks[index].offset + size;
                        blocks[index+1].free = 1;
                        blocks[index+1].id = getUniqueId();
                        blocks[index].size_of_block = size;
                        return block;

                    }

                }
                else{ //The case where a new block and copy is required.


                    void* newBlock = mem_alloc(size);

                    memcpy(newBlock, blocks[index].offset + memory_pool, blocks[index].size_of_block);
                    blocks[index].free = 1;
                    return newBlock;

                }
            }

        }

    }

}

void mem_deinit(){
    /*
Frees up the memory pool that was initially allocated by the mem_init function, ensuring that all allocated memory is returned to the system.
*/

free(memory_pool);
free(blocks);

}