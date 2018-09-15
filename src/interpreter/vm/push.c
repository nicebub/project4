#include "push.h"




#define PUSH(intype, cgintyp, value)  \
	vm_stack.stacksize += 1; \
	vm_stack.top[vm_stack.stacksize-1] = NULL; \
	SWITCH(cgintyp,	\
	REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], int, cgintyp), \
	REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], float, cgintyp), \
	REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], char, cgintyp, \
		*strlen((char*)value)+1), \
	REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], intype, GENERIC)) \
	temp = vm_stack.top[vm_stack.stacksize-1]; \
	if(cgintyp == STR || cgintyp == CHAR){ \
		char* temc=(char*)temp;\
		for(int g=0;g<strlen(value);g++)\
			temc[g] = ' ';\
		temc[strlen(value)] = '\0'; \
		strlcpy((char*)temp, (char*)value, strlen(temp)+1); \
/*	dbprint(VMLIBC,"3 ways of the string: ",3, STR, temp, STR, temc, STR, vm_stack.top[vm_stack.stacksize-1]);*/\
	}\
	else \
		*(intype*)temp =  *(intype*)value; \
	vm_stack.type[vm_stack.stacksize-1] = cgintyp;




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
