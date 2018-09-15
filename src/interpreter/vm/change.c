#include "change.h"




#define CHANGE(intype, cgintyp, value,offset)  \
SWITCH(cgintyp,	\
REQUESTMEM(vm_stack.top[offset], int, cgintyp), \
REQUESTMEM(vm_stack.top[offset], float, cgintyp), \
REQUESTMEM(vm_stack.top[offset], char, cgintyp, \
*strlen((char*)value)+1), \
REQUESTMEM(vm_stack.top[offset], intype, GENERIC)) \
temp = vm_stack.top[offset]; \
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
vm_stack.type[offset] = cgintyp;


void change_stack_value(typecg inType, void * value, int offset){
    void *temp;
    void * temp1;
    void * temp2;
    temp = NULL;
    temp1 = NULL;
    temp2 = NULL;
    
    temp1 =  vm_stack.top[vm_memory.current_bp[vm_memory.current_scope]];
    vm_stack.top[vm_memory.current_bp[vm_memory.current_scope]]=NULL;
    
    if(value != NULL && vm_stack.stacksize < STACKSIZE){
	   SWITCH(inType,CHANGE(int, INT, value,offset),\
			CHANGE(float, FLOAT, value,offset),\
			CHANGE(char, STR, value,offset),\
			CHANGE(unsigned long, LONG, value,offset))
    }

}
