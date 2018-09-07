#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "vm.h"
#include "stdlib.h"
#include "memlib.h"
#include "debuglib.h"
#include <stdio.h>
#include <string.h>

stack vm_stack;
memory vm_memory;

void initialize_machine(){
	vm_stack.stacksize=0;
	for(int i=0;i<STACKSIZE;i++){
		vm_stack.top[i]= NULL;
		vm_stack.type[i]=GENERIC;
		vm_memory.tag[i] = -1;
		vm_memory.address[i] = 0;
		vm_memory.offset[i] = -1;
		vm_memory.leveldiff[i] = -1;
	}
}

void push(typecg inType, void* value, ...){
	void *temp;
	temp = NULL;
	if(value != NULL && vm_stack.stacksize < STACKSIZE){
		SWITCH(inType,PUSH(int, INT, value),\
			PUSH(float, FLOAT, value),\
			PUSH(char, STR, value),\
			PUSH(unsigned long, LONG, value))
	}
}

void* pop(typecg *intype, int number){
	if(number == -1 || vm_stack.stacksize == 0){
		return NULL;
	}
	void* temp = vm_stack.top[vm_stack.stacksize-1];
	vm_stack.top[vm_stack.stacksize-1] = NULL;
	*intype = vm_stack.type[vm_stack.stacksize-1];
	vm_stack.stacksize -= 1;
	return temp;
}

void showstack(){
	dbprint(0,"-------------------------------------", 0);
	dbprint(0, "---stacksize----", 1, INT, vm_stack.stacksize);
	for(int i=vm_stack.stacksize-1; i >= 0;i--){
		dbprint(0, "---showstack iteration----", 1, INT, i, STR, "-----");
		dbprint(0,"--",1, STR,(void*)typecg_strings[vm_stack.type[i]]);
		SWITCH(vm_stack.type[i], dbprint(0,"--",1, INT,*(int*) vm_stack.top[i]), \
			dbprint(0,"--",1, FLOAT,*(float*)vm_stack.top[i]), \
			dbprint(0,"--",1, STR,(void*)vm_stack.top[i]), \
			dbprint(0,"--",1, LONG,(unsigned long*)vm_stack.top[i]))
			dbprint(0,"-------------------------------------",0);
	}
}
