#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "store.h"

typecg used_type5, used_type6;




void store(void* value, typecg intype, int inscope, int *offset){
    void *tempmem;
    tempmem = NULL;
    if(*offset < STACK_SIZE && value != NULL){
		    if(vm_memstack.stacksize < STACK_SIZE){
			   change_stack_value_at_offset(&vm_memstack,*offset-5,value,intype);
		    }
		    else{
			   printf("not supposed to happen\n");
			   exit(-1);
		    }
    }
    else{
	   printf("error with storing\n");
	   exit(-1);
    }
}


void in_loop_store(typecg intype){
   int * tpop1;
   int * tpop2;
   tpop1 = NULL;
   tpop2 = NULL;
   tpop1 = pop(&used_type5,0);
   tpop2 = pop(&used_type6,1);

	#ifdef DEBUG
	dbprint(VMLIBC,"first to be popped",1, INT, tpop1);
	dbprint(VMLIBC,"second to be popped",1 ,INT, tpop2);
	#endif
    switch(intype){
	   case INT:
		  	change_stack_value_at_offset(&vm_memstack,*tpop2,tpop1,INT);
		  break;
	   case FLOAT:
		  change_stack_value_at_offset(&vm_memstack,*tpop2, tpop1,FLOAT);
		  break;
	   default:
		  break;
    }
	
}
