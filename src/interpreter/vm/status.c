#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "status.h"


void showstack(void){
/*	dbprint(VMLIBC,"-------------------------------------", 0);
	dbprint(VMLIBC, "---stacksize----", 1, INT, vm_stack.stacksize);
	for(int i=vm_stack.stacksize-1; i >= 0;i--){
		dbprint(VMLIBC, "---showstack iteration----", 1, INT, i, STR, "-----");
		dbprint(VMLIBC,"--",1, STR,(void*)typecg_strings[vm_stack.type[i]]);
		SWITCH(vm_stack.type[i], dbprint(VMLIBC,"--",1, INT,*(int*) vm_stack.top[i]), \
			dbprint(VMLIBC,"--",1, FLOAT,*(float*)vm_stack.top[i]), \
			dbprint(VMLIBC,"--",1, STR,(void*)vm_stack.top[i]), \
			dbprint(VMLIBC,"--",1, LONG,(unsigned long*)vm_stack.top[i]))
			dbprint(VMLIBC,"-------------------------------------",0);
	}*/
}

void showmemory(void){
/*    dbprint(VMLIBC,"-------------------------------------", 0);
    dbprint(VMLIBC, "---memory----", 0);
    for(int i=0;i<vm_memory.total_scopes; i++){
	   dbprint(VMLIBC, "---showmemory scope ----", 1, INT, i);
	   for(int j=0;j<=vm_memory.current_set_offset[i];j++){
		  if(vm_memory.yourmem[i][j] != NULL){
		  	dbprint(VMLIBC,"--- showmemory allocated spot", 1, INT, j);
		  	dbprint(VMLIBC,"--",1, STR,(void*)typecg_strings[vm_memory.type[i][j]]);
			 SWITCH(vm_memory.type[i][j],
				   dbprint(VMLIBC,"--",1, INT,*(int*) vm_memory.yourmem[i][j]), \
				   dbprint(VMLIBC,"--",1, FLOAT,*(float*)vm_memory.yourmem[i][j]), \
				   dbprint(VMLIBC,"--",1, STR,(void*)vm_memory.yourmem[i][j]), \
				   dbprint(VMLIBC,"--",1, LONG,(unsigned long*)vm_memory.yourmem[i][j]))
			 dbprint(VMLIBC,"-------------------------------------",0);
		  }
		  else{
			 dbprint(VMLIBC,"--empty at location ",1, INT, j);

		  }
	   }
    }*/
}

