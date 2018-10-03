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


inline void fetch(typecg intype){
    int * first;
    int *second;
    int *third;
    void * newval;
    first = NULL;
    second = NULL;
    third = NULL;
    newval = NULL;
    first = (int*) pop_off_stack(&vm_memstack,&used_type7);
    

	#ifdef DEBUG
    	dbprint(VMLIBC,"WHAT IS THE FIRST ELEMENT POPPED ",1, INT, first);
	#endif
    switch(intype){
	   case INT:
		 REQUESTMEM(newval,int,INT);
		 *(int*)newval = *(int*)get_value_at_offset(&vm_memstack,*(int*)first);
		  break;
	   case FLOAT:
		  REQUESTMEM(newval,float,FLOAT);
		 *(float*)newval = *(float*)get_value_at_offset(&vm_memstack,*(int*)first);
		  break;
	   case STR:
	   case CHAR:
		  REQUESTMEM(newval,char,STR);
		 strcpy(newval,(char*)get_value_at_offset(&vm_memstack,*(int*)first));
		  break;
	   default:
		  REQUESTMEM(newval,char*,STR);
		  break;
    }
//		 *newval = (void*)get_value_at_offset(&vm_memstack,*(int*)first);
		  push_onto_stack(&vm_memstack,newval,intype);
}
