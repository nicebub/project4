#ifndef VMS_H
#define VMS_H
#define STACKSIZE 800
#include "typeint.h"
#define MEMORY_SIZE 500

#define PUSH(intype, cgintyp, value)  \
		vm_stack.stacksize += 1; \
		SWITCH(cgintyp,	\
			REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], intype, cgintyp), \
			REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], intype, cgintyp), \
			REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], intype, cgintyp, \
				*strlen((char*)value)+1), \
			REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], intype, GENERIC)) \
		temp = vm_stack.top[vm_stack.stacksize-1]; \
		if(cgintyp == STR || cgintyp == CHAR) \
			strlcpy((char*)temp, (char*)value, sizeof(intype)*strlen((char*)value)+1); \
		else \
			*(intype*)temp =  *(intype*)value; \
		vm_stack.type[vm_stack.stacksize-1] = cgintyp; 

#define SWITCH(intype,forint,forfloat,forstr,fordef) \
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
		 }



typedef struct _memory memory;
struct _memory{
	int tag[MEMORY_SIZE];
	unsigned long address[MEMORY_SIZE];
	int offset[MEMORY_SIZE];
	int leveldiff[MEMORY_SIZE];
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
void initialize_machine(void);

#endif
