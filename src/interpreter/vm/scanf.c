#include "scanf.h"


void mscanf(int count){
#ifdef DEBUG
dbprint(VMLIBC,"THIS IS A SCANF",0);
#endif
if(count > 0){
  char * infmt;
  void * invalue;
  void * newval;
    char* currentchar;

  infmt = NULL;
  invalue = NULL;
  newval = NULL;
  currentchar = NULL;

    invalue = pop_off_stack(&vm_memstack,&used_type6);
    infmt =   pop_off_stack(&vm_memstack,&used_type7);

    currentchar = (char*)infmt;
    currentchar++;
    switch(*currentchar){
	   case 'd':
		  REQUESTMEM(newval,int,INT)
		  scanf(infmt,newval);
		  change_stack_value_at_offset(&vm_memstack,*(int*)invalue,newval,INT);
		  #ifdef DEBUG
		  dbprint(VMLIBC,"newvalue ", 1, INT, (int*)newval);
		  dbprint(VMLIBC,"invalue: ", 1, INT, *(int*)invalue);
		  #endif
		  break;
		  
	   case 'f':
		  REQUESTMEM(newval,float,FLOAT)
		  scanf(infmt,newval);
		  change_stack_value_at_offset(&vm_memstack,*(int*)invalue,newval,FLOAT);
		  #ifdef DEBUG
		  dbprint(VMLIBC,"newvalue ", 1, FLOAT, (float*)newval);
		  dbprint(VMLIBC,"invalue: ", 1, INT, *(int*)invalue);
		  #endif
		  break;
		  
		  
	   case 's':
		  REQUESTMEM(newval,char, STR, *MAX_STR_CONST)
		  scanf(infmt,newval);
		  change_stack_value_at_offset(&vm_memstack,*(int*)invalue,newval,STR);
		  #ifdef DEBUG
		  dbprint(VMLIBC,"newvalue ", 1, STR, (char*)newval);
		  dbprint(VMLIBC,"invalue: ", 1, INT, *(int*)invalue);
		  #endif
		  break;
		  
    }
}
}
