#include "return.h"


typecg used_type7;


int returnf(translation_unit ** other_units,int *c,int *commandnum, translation_unit** current_unit,commandlisttype** currentcommand){
	char * unit_name;
	int * cmdnum;
    translation_unit * current_unitt;
    commandlisttype * currentcommandt;
    current_unitt = NULL;
    currentcommandt = NULL;
	unit_name = NULL;
	cmdnum = NULL;
//	int *returnval;
	//returnval =  (int*) pop(&used_type5,3);
	unit_name =  (char*)pop(&used_type6,1);
	cmdnum = 	(int*) pop(&used_type7,2);
	int e;
	for(e=0;e<MAX_FUNCTIONS && other_units[e] != NULL;e++){
		if(strcmp(other_units[e]->name,unit_name) == 0){
			break;
 	   	}	
	}
	if(e < MAX_FUNCTIONS && other_units[e] != NULL){
		*c=e;
		commandnum = cmdnum;
		current_unitt = other_units[e];
  		currentcommandt= current_unitt->commandlist->list;
		for(int j=1;j<=*commandnum;j++)
	    	currentcommandt = currentcommandt->nextcommand;
/*	    for(int r=vm_memory.used_stack[vm_memory.current_scope]; r>0; r--){
		   pop(&used_type5,1);
	    }*/
	    vm_memory.current_scope -=1;
	    vm_memory.total_scopes -=1;
	   *current_unit = current_unitt;
	    *currentcommand = currentcommandt;
		return 1;
	}
	else{
  		printf("error not happening\n");
 		exit(-1);
	}	
}
