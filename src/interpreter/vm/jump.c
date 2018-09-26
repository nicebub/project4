#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "jump.h"

int jump(boolcg z, translation_unit ** other_units, commandlisttype ** currentcommand, translation_unit ** current_unit, int * commandnum, int * c){
int * conditional;
translation_unit * current_unitt;
commandlisttype * currentcommandt;
current_unitt = NULL;
currentcommandt = NULL;
conditional = NULL;
//current_unitt = *current_unit;
currentcommandt = *currentcommand;
switch(z){
	case TRUE: {
	    conditional = pop(&used_type5,1);
		if(*conditional == 0){
			int k;
			for(k=0;k< MAX_FUNCTIONS && other_units[k] != NULL;k++){
				if(strcmp(currentcommandt->paramlist->list->val[0],other_units[k]->name)==0){
					current_unitt = other_units[k];
					currentcommandt = current_unitt->commandlist->list;
					*commandnum = 1;
					*c=k;
					*current_unit = current_unitt;
					*currentcommand = currentcommandt;
					break;
				}
			}
			if(k < MAX_FUNCTIONS && other_units[k] !=NULL){
				return 1;
			}
		}
		return 0;
	}
	case FALSE: {
		int k;
		for(k=0;k< MAX_FUNCTIONS && other_units[k] != NULL;k++){
			if(strcmp(currentcommandt->paramlist->list->val[0],other_units[k]->name)==0){
				current_unitt = other_units[k];
				currentcommandt = current_unitt->commandlist->list;
				*commandnum = 1;
				*c=k;
				*current_unit = current_unitt;
				*currentcommand = currentcommandt;
				break;
			}
		}
		if(k < MAX_FUNCTIONS && other_units[k] !=NULL){
			return 1;
		}
		
	}
}
    return 0;
}
