
#ifndef MEMORY_MANAGER_H

#define MEMORY_MANAGER_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void mem_init(size_t size);
void* mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit();
void print_all_block();
int getUniqueId();
struct metadata* findNextFreeBlock(struct metadata* block, size_t size);
struct metadata {
    size_t size_of_block;
    int free;
    struct metadata* pos_next_block;
    int id;
};

#endif