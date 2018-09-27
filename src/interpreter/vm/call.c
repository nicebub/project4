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
	   mscanf(count);
    }
    else if(strcmp(inname,"$printf") == 0){
		mprintf(count);
    }
}


int in_loop_call(commandlisttype **currentcommand,translation_unit** other_units,\
    int *commandnum, translation_unit** current_unit, int *c){
    commandlisttype * currentcommandt;
    translation_unit* current_unitt;
    activationrecord inrecord;
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
				void * tempvalue[10];
			    typecg temptype[10];
				int * in_off;
				in_off = NULL;
			    for(int g=0;g<10;g++){
				   tempvalue[g] = NULL;
				   temptype[g] = GENERIC;
			    }
				REQUESTMEM(in_off,int,INT)
				for(int g=0;g<currentcommandt->paramlist->list->int_val[1];g++){
				    	tempvalue[g] = pop(&used_type5,3);
				    temptype[g] = used_type5;
					#ifdef DEBUG
					#endif
					*in_off = *in_off +1;
				 }
			    init_activation_record(&inrecord);
			    inrecord.access_link = (char*)&vm_memstack.stack[0];
			    inrecord.alloc_amount = currentcommandt->paramlist->list->int_val[1];
			    inrecord.last_command_instruction = *commandnum;
			    strcpy(inrecord.last_command_name,current_unitt->name);
			    strcpy(inrecord.returnvalue,"0.0");
			    push_activation_record(&vm_memstack,inrecord);
			    for(int g=inrecord.alloc_amount-1;g>=0;g--)
				   push_onto_stack(&vm_memstack,tempvalue[g],temptype[g]);
				 current_unitt = other_units[d];
				 currentcommandt = current_unitt->commandlist->list;
			    *currentcommand = currentcommandt;
			    *current_unit=current_unitt;
				 *commandnum = 1;
				 *c =d;
				 return 1;
			}
			return -1;		  
		}
	
	
}
