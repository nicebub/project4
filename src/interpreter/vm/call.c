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

/*
void call_other(char * inname, int count, ...){
    
}
*/
/*
inline void call(char * inname, int count, ...){
    if(strcmp(inname,"$scanf") ==0){													// Library call 1  --- strcmp && Comparison 1
	   mscanf(count);																	// extra calculations
    }
    else if(strcmp(inname,"$printf") == 0){												// Library call 2  --- strcmp && Comparison 2
		mprintf(count);																	// extra calculations
    }
}
*/

inline int in_loop_call(commandlisttype **currentcommand,translation_unit** other_units,\
    int *commandnum, translation_unit** current_unit, int *c){
    commandlisttype * currentcommandt;
    translation_unit* current_unitt;
    activationrecord inrecord;
    current_unitt = NULL;
    currentcommandt = NULL;
    currentcommandt = *currentcommand;
    current_unitt = *current_unit;
    int count = currentcommandt->paramlist->list->int_val[1];

		if(strcmp(currentcommandt->paramlist->list->val[0],"$scanf")==0){				// Library call 1  --- strcmp && Comparison 1
			mscanf(count);																// extra calculations
			return 0;
		}
		else if(strcmp(currentcommandt->paramlist->list->val[0],"$printf")== 0){		// Library call 2  --- strcmp && Comparison 2
			mprintf(count);																// extra calculations
			  return 0;
		}				  
	   	else{
			int d;
			for(d=0;d<MAX_FUNCTIONS && other_units[d] != NULL;d++){						// Comparison 3 to (MAX_FUNCTIONS-1)*2
				if(strcmp(currentcommandt->paramlist->list->val[0],other_units[d]->name)==0){	// Library call 3  --- strcmp && Comparison +
					break;
				}
			}
			if(other_units[d] != NULL){													// Comparison +
				void * tempvalue[10];
			    typecg temptype[10];
				int * in_off;
				in_off = NULL;
			    for(int g=0;g<10;g++){													// Comparison + 10
				   tempvalue[g] = NULL;
				   temptype[g] = GENERIC;
			    }
				REQUESTMEM(in_off,int,INT)												// extra calculations
				for(int g=0;g<currentcommandt->paramlist->list->int_val[1];g++){		// Comparison + 0 through a value
				    	tempvalue[g] = pop(&used_type5,3);								// extra calculations
				    temptype[g] = used_type5;
					#ifdef DEBUG
					#endif
					*in_off = *in_off +1;
				 }
			    init_activation_record(&inrecord);										// extra calculations
			    inrecord.access_link = (char*)&vm_memstack.stack[0];
			    inrecord.alloc_amount = currentcommandt->paramlist->list->int_val[1];
			    inrecord.last_command_instruction = *commandnum;
			    strcpy(inrecord.last_command_name,current_unitt->name);					// Library call 4  --- strcpy
			    strcpy(inrecord.returnvalue,"0.0");										// Library call 5  --- strcpy
			    push_activation_record(&vm_memstack,inrecord);							// extra calculations
			    for(int g=inrecord.alloc_amount-1;g>=0;g--)								// Comparison + alloc_amount
				   push_onto_stack(&vm_memstack,tempvalue[g],temptype[g]);				// extra calculations
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
