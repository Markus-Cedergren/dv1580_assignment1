#include "memory_manager.h"
#include <stdio.h>


int main(void){
    printf("Creating memory with 1000 bytes \n");
    mem_init(1000);
    print_all_block();
    printf("Allocating 200 \n");
    void* p1 = mem_alloc(200);
    print_all_block();
    printf("Allocating 100 \n");
    void* p2 = mem_alloc(100);
    print_all_block();
    printf("Deallocating \n");
    mem_free(p1);

    print_all_block();
    
    return 0;
}



