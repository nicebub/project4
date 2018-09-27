#include "pop.h"

void* pop(typecg *intype, int number){
	if(number == -1 || vm_memstack.stacksize == 0){
		return NULL;
	}
    return pop_off_stack(&vm_memstack,intype);
}
