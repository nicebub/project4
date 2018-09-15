#ifndef MEMORY_H
#define MEMORY_H

#include "../typeint.h"

#define MEMORY_SIZE 100
#define SCOPE_SIZE 10


typedef struct _memory memory;
struct _memory{
//    int tag[MEMORY_SIZE];
  //  unsigned long address[MEMORY_SIZE];
//    int offset[MEMORY_SIZE];
//    int leveldiff[MEMORY_SIZE];
    void *yourmem[SCOPE_SIZE][MEMORY_SIZE];
    int set[SCOPE_SIZE][MEMORY_SIZE];
    int current_set_offset[SCOPE_SIZE];
    int frame_size[SCOPE_SIZE];
    int num_param[SCOPE_SIZE];
    int used_stack[SCOPE_SIZE];
    int current_bp[SCOPE_SIZE];
    typecg type[SCOPE_SIZE][MEMORY_SIZE];
    int current_scope;
    int total_scopes;
};


extern memory vm_memory;

#endif
