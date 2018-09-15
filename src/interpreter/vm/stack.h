#ifndef STACK_H
#define STACK_H

#include "../typeint.h"

#define STACKSIZE 100


typedef struct _stack stack;
struct _stack {
	void *top[STACKSIZE];
	typecg type[STACKSIZE];
	int stacksize;
};

extern stack vm_stack;

#endif
