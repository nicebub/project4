#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "enter.h"

void enter(int inscope){
	#ifdef DEBUG
    dbprint(VMLIBC,"FOUND AN ENTER IN MACHINE",0);
    dbprint(VMLIBC,"CURRENT SCOPE: ",1,INT, vm_memory.current_scope);
	#endif

    if(inscope < SCOPE_SIZE){
	   if(inscope == 0){
		  vm_memory.current_scope = 1;
	   }
	   else{
		  if(inscope <= vm_memory.total_scopes){
			 vm_memory.current_scope = inscope;
		  }
		  else if(inscope == vm_memory.total_scopes +1){
			 vm_memory.current_scope = inscope;
			 vm_memory.total_scopes +=1;
		  }
	   }
    }
    else{
    }
	#ifdef DEBUG
    dbprint(VMLIBC,"CURRENT SCOPE after change: ",1,INT, vm_memory.current_scope);
	#endif

}
