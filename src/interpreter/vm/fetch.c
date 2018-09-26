#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "fetch.h"


void fetch(typecg intype){
    int * first;
    int *second;
    int *third;
    first = NULL;
    second = NULL;
    third = NULL;
    first = (int*) pop_off_stack(&vm_memstack,&used_type7);
#ifdef DEBUG
    dbprint(VMLIBC,"WHAT IS THE FIRST ELEMENT POPPED ",1, INT, first);
#endif
/*    if(*first == -420){
	   switch(intype){
		  case INT:
			 second = pop_off_stack(&vm_memstack);
			 third = pop_off_stack(&vm_memstack);
		//	 push(intype, vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
			#ifdef DEBUG
			 #endif
			 	break;
		  case FLOAT:
			 second = pop_off_stack(&vm_memstack);
			 third = pop_off_stack(&vm_memstack);
	//		 push(intype, vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
			#ifdef DEBUG
			#endif
			 	break;
		  default: break;
	   }
    }*/
//    else{
	   push_onto_stack(&vm_memstack,get_value_at_offset(&vm_memstack,*(int*)first),intype);
#ifdef DEBUG
#endif
//    }
}
