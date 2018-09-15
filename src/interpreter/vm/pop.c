#include "pop.h"
extern stack vm_stack;
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
