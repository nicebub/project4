#include "return.h"


typecg used_type7;


inline int returnf(translation_unit ** other_units,int *c,int *commandnum, translation_unit** current_unit,commandlisttype** currentcommand){
	char * unit_name;
	int * cmdnum;
    translation_unit * current_unitt;
    commandlisttype * currentcommandt;
    current_unitt = NULL;
    currentcommandt = NULL;
	unit_name = NULL;
	cmdnum = NULL;

    unit_name =  (char*) get_cell_value(vm_memstack.bp+1);
	cmdnum = 	(int*) get_value_at_offset_n_frames_back(&vm_memstack,4,0);
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

	    pop_activation_record(&vm_memstack,&used_type5);
	    

	    *current_unit = current_unitt;
	    *currentcommand = currentcommandt;
		return 1;
	}
	else{
  		printf("error not happening\n");
 		exit(-1);
	}	
}
