#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "initialize.h"

typecg *used_type1;
typecg *used_type2;
typecg *used_type3;


void initialize_machine(void){
    boolcg hasrun;
    used_type1 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type1 = INT;
    used_type2 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type2 = FLOAT;
    used_type3 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type3 = STR;
    vm_stack.stacksize=0;
    vm_memory.current_scope= 0;
    vm_memory.total_scopes = 1;
    hasrun = FALSE;
    for(int i=0,j=0;i<STACKSIZE && j<MEMORY_SIZE;i++,j++){
		vm_stack.top[i]= NULL;
		vm_stack.type[i]=GENERIC;
//		vm_memory.tag[j] = -1;
//		vm_memory.address[j] = 0;
//		vm_memory.offset[j] = -1;
//		vm_memory.leveldiff[j] = -1;
	   for(int k = 0; k<SCOPE_SIZE;k++){
	   	//vm_memory.yourmem[k][j] = NULL;
	     //vm_memory.set[k][j] = 0;
	     vm_memory.type[k][j] = GENERIC;
		  if(hasrun == FALSE){
			 vm_memory.current_set_offset[k] = 0;
			 vm_memory.frame_size[k]= 0;
			 vm_memory.num_param[k]= 0;
			 vm_memory.used_stack[k]=0;
			 vm_memory.current_bp[k] =0;

		  }
	   }
	   hasrun = TRUE;
	}
}
