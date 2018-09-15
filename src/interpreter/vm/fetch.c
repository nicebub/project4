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
    first = (int*) pop(&used_type5, 1);
#ifdef DEBUG
    dbprint(VMLIBC,"WHAT IS THE FIRST ELEMENT POPPED ",1, INT, first);
#endif
    if(*first == -420){
	   switch(intype){
		  case INT:
			 second = pop(&used_type6,2);
			 third = pop(&used_type7,4);
			 push(intype, vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#ifdef DEBUG
			 dbprint(VMLIBC,"memory in fetch: ", 1, INT, *(int*)vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#endif
			 	break;
		  case FLOAT:
			 second = pop(&used_type6,3);
			 third = pop(&used_type7,4);
			 push(intype, vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#ifdef DEBUG
			 dbprint(VMLIBC,"memory in fetch: ", 1, INT, *(int*)vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#endif
			 	break;
		  default: break;
	   }
    }
    else{
	   push(intype, vm_memory.yourmem[vm_memory.current_scope][(*first)-5]);
#ifdef DEBUG
	   dbprint(VMLIBC,"memory in fetch: ", 1, INT, *(int*)vm_memory.yourmem[vm_memory.current_scope][(*(int*)first)-5]);
#endif
    }
}
