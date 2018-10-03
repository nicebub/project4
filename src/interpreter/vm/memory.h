#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../typeint.h"

#define STACK_SIZE 40000
#define MAX_SIZE 8

#ifndef MAX_STR_CONST
#define MAX_STR_CONST 50
#endif


typedef max_align_t alignme;

union memcell {
    alignme a;
    struct {
	   	union {
		    int num;
		    float real;
		    char character;
		    char * string;
		    void * address;
    		} s;
	   typecg type;
	   size_t size;
    } m;
};

typedef union memcell memory_cell;


typedef struct _memstack memstack;
struct _memstack {
     memory_cell stack[STACK_SIZE];
	memory_cell	* bp;
	memory_cell	* sp;
	size_t		stacksize;
};

typedef struct _activationrecord activationrecord;
struct _activationrecord {
	char	last_command_name[MAX_STR_CONST];
	char	returnvalue[MAX_SIZE];
	char	* control_link;
	char	* access_link;
	int		last_command_instruction;
	int		alloc_amount;
};


extern memstack vm_memstack;
extern activationrecord vm_temp_record;

void init_memory_cell(memory_cell *incell);
void reinit_memory_cell(memory_cell *incell);

void set_cell_size(memory_cell *incell, size_t s);
void set_cell_type(memory_cell *incell,typecg t);
void set_cell_value(memory_cell *incell,void * v, typecg t, size_t s);
size_t get_cell_size(memory_cell *incell);
typecg get_cell_type(memory_cell *incell);
void* get_cell_value(memory_cell *incell);


void init_memstack(memstack *instack);
void delete_stack(memstack *instack);
void push_onto_stack(memstack *instack, void * value, typecg intype);
void* pop_off_stack(memstack *instack, typecg * outtype);
void delete_stack(memstack *instack);

boolcg stack_isempty(memstack *instack);
size_t get_stack_size(memstack *instack);

void change_stack_value_at_offset(memstack *instack, int offset, void* value, typecg intype);
void change_stack_value_at_offset_n_frames_back(memstack *instack, int offset, int n,void* value, typecg intype);


void init_activation_record(activationrecord *inrecord);

void push_activation_record(memstack *instack, activationrecord inrecord);
void * pop_activation_record(memstack *instack,typecg *out_return_type);

void * get_value_at_offset(memstack *instack,int offset);
void * get_value_at_offset_n_frames_back(memstack *instack,int offset,int n);

void print_stack(memstack *instack);
void print_memory_cell(memory_cell *incell);

int get_address(memstack *instack, int leveldiff, int offset);

#endif
