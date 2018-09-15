#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "call.h"

void call_other(char * inname, int count, ...){
    
}

void call(char * inname, int count, ...){
    if(strcmp(inname,"$scanf") ==0){
		#ifdef DEBUG
	   dbprint(VMLIBC,"THIS IS A SCANF",0);
		#endif
//	   va_list arglist;
//	   va_start(arglist,count);
	   if(count > 0){
		  char * infmt;
		  void * invalue;
		  void * newval;
		  infmt = NULL;
		  invalue = NULL;
		  newval = NULL;
//			    infmt = va_arg(arglist,char*);
//		  		invalue = va_arg(arglist, void*);
			    invalue = pop(&used_type5,3);
			    infmt =   pop(&used_type6,4);
			    vm_memory.used_stack[vm_memory.current_scope] -= 2;
			    
			    REQUESTMEM(newval,char,STR, *MAX_STR_CONST)
			    scanf(infmt,newval);
#ifdef DEBUG
			    dbprint(VMLIBC,"newvalue ", 1, INT, *(int*)newval);
			    dbprint(VMLIBC,"invalue: ", 1, INT, *(int*)invalue);
#endif
			    store((int*)newval,INT,vm_memory.current_scope,(int*)invalue);
//			    vm_memory.yourmem[vm_memory.current_scope][*((int*)invalue)-5]);
			
	   }
//	   va_end(arglist);

    }
    else if(strcmp(inname,"$printf") == 0){
		mprintf(count);
    }
}


int in_loop_call(commandlisttype **currentcommand,translation_unit** other_units,\
    int *commandnum, translation_unit** current_unit, int *c){
    commandlisttype * currentcommandt;
    translation_unit* current_unitt;
    current_unitt = NULL;
    currentcommandt = NULL;
    currentcommandt = *currentcommand;
    current_unitt = *current_unit;
		if(strcmp(currentcommandt->paramlist->list->val[0],"$scanf")==0 || \
					strcmp(currentcommandt->paramlist->list->val[0],"$printf")== 0){
				   		call(currentcommandt->paramlist->list->val[0],
						  currentcommandt->paramlist->list->int_val[1]);
						  return 0;
					  }
						  
	   	else{
			int d;
			for(d=0;d<MAX_FUNCTIONS && other_units[d] != NULL;d++){
				if(strcmp(currentcommandt->paramlist->list->val[0],other_units[d]->name)==0){
					break;
				}
			}
			if(other_units[d] != NULL){
				void * tempvalue;
				int * in_off;
				typecg intype;
//				void * stack_top;
				in_off = NULL;
				tempvalue = NULL;
				REQUESTMEM(in_off,int,INT)
//				stack_top = NULL;
//				stack_top = vm_stack.top[vm_stack.stacksize-1];
				for(int g=0;g<currentcommandt->paramlist->list->int_val[1];g++){
				    	intype = vm_stack.type[(vm_stack.stacksize-1)-g];
				    	tempvalue = pop(&used_type5,3);
//					tempvalue = vm_stack.top[(vm_stack.stacksize-1)-g];
					#ifdef DEBUG
					dbprint(VMLIBC,"looking at type in call instruction ",1, STR, typecg_strings[intype]);
					#endif
					*in_off = vm_memory.current_set_offset[vm_memory.current_scope+1] +5;
					store(tempvalue, intype,vm_memory.current_scope+1,in_off);
					*in_off = *in_off +1;
					vm_memory.current_set_offset[vm_memory.current_scope+1] += 1;
				 }
			    vm_memory.num_param[vm_memory.current_scope] = \
			    	currentcommandt->paramlist->list->int_val[1];
			    push(FLOAT,(double[]){ 0.0 });
			    vm_memory.current_bp[vm_memory.current_scope+1]=vm_stack.stacksize-1;
				 push(INT,(int[]) { *commandnum });
				 push(STR,current_unitt->name);
				 current_unitt = other_units[d];
				 currentcommandt = current_unitt->commandlist->list;
			    *currentcommand = currentcommandt;
			    *current_unit=current_unitt;
				 *commandnum = 1;
				 *c =d;
				 vm_memory.current_scope +=1;
				 vm_memory.total_scopes +=1;
				 return 1;
			}
			return -1;		  
					 // call_other(currentcommand->paramlist->list->val[0],
						//	   currentcommand->paramlist->list->int_val[1]);
		}
	
	
}
