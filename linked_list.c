#include "linked_list.h"
#include "memory_manager.h"
#include <stdlib.h>
#include <stdio.h>



void list_init(Node** head, size_t size){
    mem_init(size);
    *head = NULL;
}

void list_insert(Node** head, uint16_t data){
    Node* newNode = (Node* )mem_alloc(sizeof(Node)); //add a new node.
    newNode->data = data; //add the data
    newNode->next = NULL; //next is unknow since we add at tail.

    if(*head == NULL){ //if its the first node, set head to newNode (beginning of list)
        *head = newNode;
    }else{ //Iterate to find the last node. Set "next" of the last node to the newNode.
        Node* currentNode = *head;
        while (currentNode->next != NULL){
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
}

void list_insert_after(Node* prev_node, uint16_t data){
    
    Node* newNode = (Node*)mem_alloc(sizeof(Node));
    newNode->data = data;

    Node* prev_nodeNext = prev_node->next;
    prev_node->next = newNode;
    newNode->next = prev_nodeNext;
    

}

void list_insert_before(Node** head, Node* next_node, uint16_t data){

    Node* newNode = (Node*)mem_alloc(sizeof(Node));
    newNode->data = data;

    Node* currentNode = *head;
    while(currentNode->next != next_node){
        currentNode = currentNode->next;
    }
    currentNode->next = newNode;
    newNode->next = next_node;

}

void list_delete(Node** head, uint16_t data){
    Node* currentNode = *head;

    if((*head)->data == data){ //handles the case where the node to remove is the first node.
        *head = (*head)->next;
    }
    else if(*head == NULL){ //if its an empty list
        return;
    }
    else{ //find the node and chain the previous with the next. 
        while(currentNode->next->data != data && currentNode->next !=NULL ){
        currentNode = currentNode->next;
    }

    Node* nextNext = currentNode->next->next;
    currentNode->next = nextNext;
    }
    
}

Node* list_search(Node** head, uint16_t data){
    Node* currentNode = *head;
    while (currentNode != NULL && currentNode->data != data){
        currentNode = currentNode->next;
    }

    if(currentNode == NULL){
        return NULL;
    }
    else{
        return currentNode;
    }
}

void list_display(Node** head){

    if(*head == NULL){
        printf("[]");
        return;
    }
    Node* currentNode = *head;

    
    printf("[%d", currentNode->data);
    currentNode =currentNode->next;
    while(currentNode->next != NULL){
        printf(", %d", currentNode->data);
        currentNode = currentNode->next;
    }
    printf(", %d]", currentNode->data);
}

void list_display_range(Node** head, Node* start_node, Node* end_node){
    if(*head == NULL){
        printf("[]");
        return;
    }

    

    if(start_node == NULL && end_node == NULL){ // print whole list
    Node* currentNode = *head;
    printf("[%d", currentNode->data);
    currentNode = currentNode->next;
    while(currentNode->next != NULL){
        printf(", %d", currentNode->data);
        currentNode = currentNode->next;
    }
    printf(", %d]", currentNode->data);

    }




    else if(start_node == NULL && end_node != NULL){//print from start of list to end_node
        Node* currentNode = *head;
        printf("[%d", currentNode->data);
        currentNode = currentNode->next;
        while(currentNode->next != end_node->next && currentNode->next != NULL){
            printf(", %d",currentNode->data);
            currentNode = currentNode->next;
        }
        printf(", %d]", currentNode->data);
        
    }
    else if(end_node == NULL && start_node != NULL){ // print from start node to end of list
        Node* currentNode = start_node;
        printf("[%d", currentNode->data);
        currentNode = currentNode->next;
        while(currentNode->next != NULL){
            printf(", %d",currentNode->data);
            currentNode = currentNode->next;
        }
        printf(", %d]", currentNode->data);
    
    }

    else{ //both start and end are specified.
        Node* currentNode = start_node;
        printf("[%d", currentNode->data);
        currentNode = currentNode->next;
        while(currentNode->next != end_node->next){
            printf(", %d", currentNode->data);
            currentNode = currentNode->next;
        }
        printf(", %d]", currentNode->data);


    }

}

int list_count_nodes(Node** head){
    int count = 0;

    Node* currentNode = *head;

    while(currentNode != NULL){
        currentNode = currentNode->next;
        count++;
    }
    return count;

}

void list_cleanup(Node** head){

    Node* currentNode = *head;

    while(currentNode != NULL){
        Node* nextNode = currentNode->next;
        mem_free(currentNode);
        currentNode = nextNode;
    }
    *head = NULL;

    mem_deinit();

}