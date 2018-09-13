#ifndef VMS_H
#define VMS_H
#include "typeint.h"
#include "Listint.h"
#define MEMORY_SIZE 100
#define STACKSIZE 100
#define SCOPE_SIZE 10

extern typecg  *used_type1, *used_type2, *used_type3;
//typecg used_type4;

		/*#define SWITCH(intype,forint,forfloat,forstr,fordef) \
		switch(intype){ \
			case INT: \
						forint; break; \
			case FLOAT: \
						forfloat; break; \
			case STR: \
			case CHAR:	 \
		 				forstr; break; \
			case REFINT: \
			case REFFLOAT: \
			case REFSTR: \
			case LONG: \
			default: \
						fordef; break; \
		 }*/


typedef struct _memory memory;
struct _memory{
    int tag[MEMORY_SIZE];
    unsigned long address[MEMORY_SIZE];
    int offset[MEMORY_SIZE];
    int leveldiff[MEMORY_SIZE];
    void *yourmem[SCOPE_SIZE][MEMORY_SIZE];
    int set[SCOPE_SIZE][MEMORY_SIZE];
    int current_set_offset[SCOPE_SIZE];
    typecg type[SCOPE_SIZE][MEMORY_SIZE];
    int current_scope;
    int total_scopes;
};

typedef struct _stack stack;
struct _stack {
	void *top[STACKSIZE];
	typecg type[STACKSIZE];
	int stacksize;
};

extern stack vm_stack;
extern memory vm_memory;



void push(typecg intype, void* value, ...);
void* pop(typecg *intype, int number);
void showstack(void);

void allocate(int inscope );
void enter(int inscope);
void store(void* value, typecg intype, int inscope,int *offset);
void fetch(typecg intype);
//void* multiply(typecg intype);
//void* add(typecg);
void * operate(char operator, typecg intype);
void * relationship(char *relation, typecg intype);
void call(char * inname, int count, ...);
void call_other(char * inname, int count, ...);

void initialize_machine(void);
void run_virtual_machine(translation_unit*,translation_unit**,int);

#endif
