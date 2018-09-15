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



#define STORE(intype, cgintyp, value,inscope,offset)  \
vm_memory.set[inscope][(*offset)-5] = 1; \
if(*offset - 5 == vm_memory.current_set_offset[inscope] + 1)\
	vm_memory.current_set_offset[inscope] += 1; \
SWITCH(cgintyp,	\
REQUESTMEM(vm_memory.yourmem[inscope][(*offset)-5], int, cgintyp), \
REQUESTMEM(vm_memory.yourmem[inscope][(*offset)-5], float, cgintyp), \
REQUESTMEM(vm_memory.yourmem[inscope][(*offset)-5], char, cgintyp, \
*strlen((char*)value)+1), \
REQUESTMEM(vm_memory.yourmem[inscope][(*offset)-5], intype, GENERIC)) \
tempmem = vm_memory.yourmem[inscope][(*offset)-5]; \
if(cgintyp == STR || cgintyp == CHAR) \
strlcpy((char*)tempmem, (char*)value, strlen(tempmem)+1); \
else \
*(intype*)tempmem =  *(intype*)value; \
vm_memory.type[inscope][(*offset)-5] = cgintyp;




void store(void* value, typecg intype, int inscope, int *offset){
    void *tempmem;
    tempmem = NULL;
    if(*offset < MEMORY_SIZE && value != NULL){
	   	if(vm_memory.set[inscope][(*offset)-5]==0){
		    if(vm_memory.current_set_offset[inscope] < MEMORY_SIZE){
			 // && (*offset)-5 <= vm_memory.current_set_offset[inscope]){
			   	SWITCH(intype,STORE(int, INT, value,inscope, offset),\
			    		STORE(float, FLOAT, value,inscope, offset),\
			    		STORE(char, STR, value,inscope, offset),\
			    		STORE(unsigned long, LONG, value,inscope, offset))
		    }
		    else{
			   printf("not supposed to happen\n");
			   exit(-1);
		    }
		}
		else{
			#ifdef DEBUG
		    dbprint(VMLIBC,"Memory region has value, overwriting",0);
			#endif
		    if(vm_memory.current_set_offset[inscope] < MEMORY_SIZE){
			   // && (*offset)-5 <= vm_memory.current_set_offset[inscope]){
			   SWITCH(intype,STORE(int, INT, value,inscope, offset),\
					STORE(float, FLOAT, value,inscope, offset),\
					STORE(char, STR, value,inscope, offset),\
					STORE(unsigned long, LONG, value,inscope, offset))
		    }
		}
    }
    else{
	   printf("error with storing\n");
	   exit(-1);
    }
}


void in_loop_store(void){
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

	store(tpop1,INT,vm_memory.current_scope,(int*)tpop2);

	
}
