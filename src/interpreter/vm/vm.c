#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif


#include "vm.h"
#include "vmlib.h"
#include "pop.h"

#include "../memlib.h"
#include "../debuglib.h"
#include "../Listint.h"
#include "../interpreter.y.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


int run_virtual_machine(translation_unit *main_unit,translation_unit **other_units,int unit_count){
    translation_unit * current_unit;
    commandlisttype *currentcommand;
    activationrecord startrecord;
    int commandnum, current_frame_size;
    int c, returnvalue;
    current_unit=other_units[0];
    current_unit=main_unit;
    c=0;
    commandnum = 1;
    current_frame_size = 1;
    commandnum=current_frame_size;
    returnvalue = 0;
    initialize_machine();
	#ifdef DEBUG
    dbprint(VMLIBC,"Before use, here are the used_type1 and 3 ",
		  2, STR, typecg_strings[*used_type1], STR, typecg_strings[*used_type3]);
	#endif

    currentcommand = main_unit->commandlist->list;
    init_activation_record(&startrecord);
    strcpy(startrecord.last_command_name,"main");
    strcpy(startrecord.returnvalue,"0");
    startrecord.alloc_amount = 1;
    startrecord.last_command_instruction =5;
    push_activation_record(&vm_memstack,startrecord);
    while(1){
	#ifdef DEBUG
	#endif
	   SWITCH_CMD(currentcommand->name_enm,
	   { /*alloc*/

		  		#ifdef DEBUG
		  		dbprint(VMLIBC,"FOUND AN ALLOC IN MACHINE",0);
		  		#endif

		  		for(int j=1;j<=currentcommand->paramlist->list->int_val[0];j++)
				    push_onto_stack(&vm_memstack,"EMPTY" ,STR);
		  		int new_amount = *(int*)get_value_at_offset_n_frames_back(&vm_memstack,5,0)+currentcommand->paramlist->list->int_val[0];
		  		change_stack_value_at_offset_n_frames_back(&vm_memstack,5,0,&new_amount,INT);
	   },
	   {/*enter*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN ENTER IN MACHINE",0);
					#endif

	   },
	   {/*pushs*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN PUSHS IN MACHINE",0);
					#endif
		  			push_onto_stack(&vm_memstack,currentcommand->paramlist->list->val[0],STR);

	   },
	   {/*call*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN CALL IN MACHINE",0);
					#endif
				   
		  if(in_loop_call(&currentcommand,other_units,&commandnum,&current_unit, &c)==1){
				#ifdef DEBUG
			 	dbprint(VMLIBC,"Printing Stack",0);
			 	print_stack(&vm_memstack);
				#endif
			 	continue;
		  }
	   },
	   {/*popI*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN POPI IN MACHINE",0);
					#endif
	   },
	   {/*pushga*/
		  			int g;
					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN PUSHGA IN MACHINE",0);
					#endif
		  
		  			g = get_address(&vm_memstack,currentcommand->paramlist->list->int_val[0],currentcommand->paramlist->list->int_val[1]);
		  			push(INT, &g);
	   },
	   {/*pusha*/
		  			int g;
					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN PUSHA IN MACHINE",0);
					#endif
		  			g = get_address(&vm_memstack,0,currentcommand->paramlist->list->int_val[0]);
		  			push(INT,&g);
	   },
	   {/*fetchI*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN FETCHI IN MACHINE",0);
					#endif

				   	fetch(INT);

	   },
	   {/*fetchR*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN FETCHR IN MACHINE",0);
					#endif

				   	fetch(FLOAT);
	   },
	   {/*storeI*/

					#ifdef DEBUG
					dbprint(VMLIBC,"FOUND AN STOREI IN MACHINE",0);
					#endif

					in_loop_store(INT);


	   },
	   {/*storeR*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN STORER IN MACHINE",0);
					#endif
		  			in_loop_store(FLOAT);

	   },
	   {/*pushcI*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN PUSHCI IN MACHINE",0);
					#endif
		  			push_onto_stack(&vm_memstack,&currentcommand->paramlist->list->int_val[0],INT);

				   
	   },
	   {/*pushcR*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN PUSHCR IN MACHINE",0);
					#endif

		  			push_onto_stack(&vm_memstack,&currentcommand->paramlist->list->float_val[0],FLOAT);
				   
	   },
	   {/*setrvI*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN SETRVI IN MACHINE",0);
					#endif
				   void * tempval;
				   tempval = NULL;
				   tempval = pop(&used_type6,3);
		  			change_stack_value_at_offset_n_frames_back(&vm_memstack,0,0,tempval,INT);
	   },
	   {/*setrvR*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN SETRVR IN MACHINE",0);
					#endif
		  			void * tempval;
		  			tempval = NULL;
		  			tempval = pop(&used_type6,3);
		  			change_stack_value_at_offset_n_frames_back(&vm_memstack,0,0,tempval,FLOAT);

	   },
	   {/*returnf*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN RETURNF IN MACHINE",0);
					#endif

				   if(returnf(other_units,&c, &commandnum,&current_unit,&currentcommand)==1){
						#ifdef DEBUG
					  dbprint(VMLIBC,"Printing Stack",0);
					  print_stack(&vm_memstack);
					   #endif

					  continue;
				   }
	   },
	   {/*return*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN RETURN IN MACHINE",0);
					#endif
		  			returnvalue = *(int*)pop_off_stack(&vm_memstack, &used_type7);
		  			pop_activation_record(&vm_memstack,&used_type6);
	   },
	   {/*ltI*/
					
					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN LTI IN MACHINE",0);
					#endif
				   	operate('<',INT);

				   
	   },
	   {/*ltR*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN LTR IN MACHINE",0);
					#endif
				   	operate('<',FLOAT);

	   },
	   {/* leI */

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN LEI IN MACHINE",0);
					#endif
				   	relationship("<=",INT);

	   },
	   {/* leR */

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN LER IN MACHINE",0);
					#endif

				   	relationship("<=",FLOAT);

	   },
        {/* gtI */
			#ifdef DEBUG
		  	dbprint(VMLIBC,"FOUND AN GTI IN MACHINE",0);
			#endif

		  operate('>',INT);

	   },
	   { /* gtR */
			#ifdef DEBUG
		  	dbprint(VMLIBC,"FOUND AN GTR IN MACHINE",0);
			#endif

		  operate('>',FLOAT);

	   },
	   { /* geI */
			#ifdef DEBUG
		  	dbprint(VMLIBC,"FOUND AN GEI IN MACHINE",0);
			#endif
		  relationship(">=",INT);

	   },
	   {  /* geR */
			#ifdef DEBUG
		  	dbprint(VMLIBC,"FOUND AN GER IN MACHINE",0);
			#endif
		  relationship(">=",FLOAT);

	   },
	   {/*jump*/
					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN JUMP IN MACHINE",0);
					#endif
				   if(jump(FALSE,other_units,&currentcommand,&current_unit,&commandnum,&c)==1)
					   continue;

	   },
	   {/*jumpz*/

				   #ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN JUMPZ IN MACHINE",0);
				   #endif

				   if(jump(TRUE,other_units,&currentcommand,&current_unit,&commandnum,&c)==1)
					   continue;
	   },
	   {/*mulI*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN MULI IN MACHINE",0);
					#endif

				   	operate('*',INT);
	   },
	   {/*mulR*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN MULR IN MACHINE",0);
					#endif
				   	operate('*',FLOAT);
	   },
	   {/*addI*/

					#ifdef DEBUG
			   		dbprint(VMLIBC,"FOUND AN ADDI IN MACHINE",0);
					#endif
				   	operate('+',INT);
	   },
	   { /*addR*/

					#ifdef DEBUG
			   		dbprint(VMLIBC,"FOUND AN ADDR IN MACHINE",0);
					#endif
				   	operate('+',FLOAT);
	   },
	   { /*subI */
			#ifdef DEBUG
		  dbprint(VMLIBC,"FOUND AN SUBI IN MACHINE",0);
			#endif
		  operate('-',INT);

	   },
	   {/* subR */
			#ifdef DEBUG
		  dbprint(VMLIBC,"FOUND AN SUBR IN MACHINE",0);
			#endif
		  operate('-',FLOAT);

	   },
	   { /* flt */
		#ifdef DEBUG
		  dbprint(VMLIBC,"FOUND AN FLT IN MACHINE",0);
		#endif
		  float invalue = (float)*(int*)get_cell_value(&vm_memstack.stack[vm_memstack.stacksize-1]);
//		  printf("float value found for flt: %f\n",invalue);
		  change_stack_value_at_offset(&vm_memstack, (int)vm_memstack.stacksize-1, &invalue, FLOAT);
	   },
	   { /* fltb */
		#ifdef DEBUG
		  dbprint(VMLIBC,"FOUND AN FLTB IN MACHINE",0);
		#endif
		  float invalue = (float)*(int*)get_cell_value(&vm_memstack.stack[vm_memstack.stacksize-2]);
	//	  printf("float value found for flt: %f\n",invalue);
		  change_stack_value_at_offset(&vm_memstack, (int)vm_memstack.stacksize-2, &invalue, FLOAT);

	   },
	   { /* int */
			#ifdef DEBUG
		  dbprint(VMLIBC,"FOUND AN INT IN MACHINE",0);
			#endif
		  int invalue = (int)*(float*)get_cell_value(&vm_memstack.stack[vm_memstack.stacksize-1]);
		  change_stack_value_at_offset(&vm_memstack, (int)vm_memstack.stacksize-1, &invalue, INT);

	   },
	   { /* intb */
			#ifdef DEBUG
		  dbprint(VMLIBC,"FOUND AN INTB IN MACHINE",0);
			#endif
		  int invalue = (int)*(float*)get_cell_value(&vm_memstack.stack[vm_memstack.stacksize-2]);
		  change_stack_value_at_offset(&vm_memstack, (int)vm_memstack.stacksize-2, &invalue, INT);

	   },
	   { /* eqI */
		  relationship("==",INT);

	   },
	   { /* eqR */
		  relationship("==",FLOAT);

	   })
	   commandnum += 1;
	   	currentcommand = currentcommand->nextcommand;
	   #ifdef DEBUG
	   dbprint(VMLIBC,"Printing Stack",0);
	   print_stack(&vm_memstack);
	   #endif
	   if(currentcommand == NULL){
		  current_unit = other_units[c+1];
		  if(current_unit == NULL)
			 	break;
		  else{
			 currentcommand = current_unit->commandlist->list;
			 c = c+1;
			 commandnum = 1;
			 continue;
		  }
	   }
    }
    return returnvalue;
    //	RETURN -- end
}

