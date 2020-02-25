#include <stdio.h>
#include <stdlib.h>

//handle freeing memory by creating delete node and delete list function

struct Node {
    int* key; //this is for the uid and the rid
    char* value;
    struct Node* next;
}

struct list {
    struct Node* head; // points to the first element of the linked list
    Struct Node* tail; // points to the last element of the linked list
}

struct list* table;
int size = 0;
int length = 16;

int hash(int* key) {
    return key % size;
}

void insert(int* key, char** value) {
    float loadFactor = 0.0; //used to check if resizing is needed or not
    int location = hash(key);
    struct Node* llist = (struct Node*) table[index].head;
    struct Node* element = (struct Node*) calloc(1,sizeof(struct Node));
    element -> key = key;
    element value = *value;
    element 
    if(!llist) {
        table[index].head = element;
        table[index].tail = element;
        size++;
    }
    else {
        int find_index = find(&llist, key); //define this function later
        if(find_index == -1) {
            table[index].tail -> next = element;
            table[index.tail] = element;
            size++;
        }
        else {
            struct Node* item = get_element(llist, find_index);
            item -> value = value;
        }
    }
    loadFactor = (1.0 * size) / max;
    if(loadFactor >= 0.5) {
        rehash(); //define later
    }
}

void rehash() {
    struct list* temp = table;
    int i = 0;
    int n = max;
    size = 0;
    max *= 2;
    table = (struct list*) calloc(max, sizeof(struct Node));
    init_table(); //define later
    for(int j = 0; j < n; j++) {
        struct Node* llist = (struct Node*) temp[i].head;
        if(!list) {
            continue;
        }
        else {
            while(list) {
                insert(list -> key, list -> value);
                list = list -> next;
            }
        }
    }
    temp = NULL;
}

int find(struct list** list, int* key) {
    int toReturn = 0;
    struct list* temp = *list;
    while(temp) {
        if(temp -> key == key) {
            return toReturn;
        }
        temp = temp -> next;
        toReturn++;
    }
    return -1;
}

struct Node* get_element(struct list** list, int* index) {
    int i = 0;
    struct list* temp = *list;
    while(i != index) {
        temp = temp -> next;
        i++;
    }
    return temp;
}

int sizeofTable() {
    return size;
}

void init_table() {
    int i = 0;
    for(int i = 0; i < max; i++) {
        table[i].head = NULL:
        table[i].tail = NULL;
    }
}
void remove_element(int* key) {
    int index = hash(key);
    struct Node* llist = (struct Node*) table[index].head;
    if(!llist) {
        return;
    }
    else {
        int find_index = find(llist, key);
        if(find_index == -1) {
            return;
        }
        else {
            struct Node* toFree;
            struct Node* temp = llist;
            if(temp -> key = key) {
                toFree = table[index].head;
                table[index].head = temp.next;
                delNode(&toFree);
            }
            while(temp -> next -> key != key) {
                temp  = temp -> next;
            }
            if(table[index].tail == temp -> next) {
                delNode((&(temp -> next));
                temp -> next = NULL;
                table[index].tail = temp;
            }
            else {
                toFree = temp -> next;
                temp -> next = temp -> next -> next;
                delNode(&toFree);
            }
        }
    }
}

void delNode(struct Node** toDel) {
    free((*(toDel)) -> key);
    free((*(toDel)) -> value);
}
