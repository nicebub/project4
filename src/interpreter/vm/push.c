#include "push.h"
#include "memory.h"

void push(typecg inType, void* value, ...){
			void *temp;
			temp = NULL;
    SWITCH(inType,
		push_onto_stack(&vm_memstack,value,INT) ,
				push_onto_stack(&vm_memstack,value,FLOAT) ,
				push_onto_stack(&vm_memstack,value,STR) ,
		 		push_onto_stack(&vm_memstack,value,REFSTR))
}

