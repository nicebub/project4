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
#include "change.h"
#include "pop.h"
//#include "push.h"

#include "../memlib.h"
#include "../debuglib.h"
#include "../Listint.h"
#include "../interpreter.y.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>


void run_virtual_machine(translation_unit *main_unit,translation_unit **other_units,int unit_count){
    translation_unit * current_unit;
    commandlisttype *currentcommand;
    int commandnum, current_frame_size;
    int c;
    current_unit=other_units[0];
    current_unit=main_unit;
    c=0;
    commandnum = 1;
    current_frame_size = 1;
    commandnum=current_frame_size;

    initialize_machine();
    vm_memory.current_scope =1;
    vm_memory.total_scopes +=1;

#ifdef DEBUG
    dbprint(VMLIBC,"Before use, here are the used_type1 and 3 ",
		  2, STR, typecg_strings[*used_type1], STR, typecg_strings[*used_type3]);
#endif
//    push(*used_type1, (int[]) { 4 });

    
 
//   push(*used_type3, last_unit->name);

    //		SET SCOPE LEVEL 0
    //	   GETMEMORY_FOR_GLOBALS(main_unit->commandlist->list->nextcommand->paramlist->list->int_val[0]);
    // 		GET_first_function()
    //		GET_first_command()
    
    
    currentcommand = main_unit->commandlist->list;
    while(1){
#ifdef DEBUG
//	   dbprint(VMLIBC,"What command are we going to run ", 1 , STR, currentcommand->name);
//	   dbprint(VMLIBC,"The command has enumeration type ", 1 , STR, command_name_strings[currentcommand->name_enm]);
#endif
	   SWITCH_CMD(currentcommand->name_enm,
	   { /*alloc*/

		  #ifdef DEBUG
			 dbprint(VMLIBC,"FOUND AN ALLOC IN MACHINE",0);
		  #endif

		  vm_memory.frame_size[vm_memory.current_scope] = currentcommand->paramlist->list->int_val[0];
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

				   push(*used_type3,(void*)currentcommand->paramlist->list->val[0]);
				   vm_memory.used_stack[vm_memory.current_scope] += 1;

	   },
	   {/*call*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN CALL IN MACHINE",0);
					#endif
				   
		  if(in_loop_call(&currentcommand,other_units,&commandnum,&current_unit, &c)==1){
				#ifdef DEBUG
			 	dbprint(VMLIBC,"Printing Memory",0);
			 	showmemory();
			 	dbprint(VMLIBC,"Printing Stack",0);
			 	showstack();
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

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHGA IN MACHINE",0);
					#endif
		  //int address;
		  //int newscope;
		  //int offset;
		  //newscope = vm_memory.current_scope - currentcommand->paramlist->list->int_val[0];
		  //offset = currentcommand->paramlist->list->int_val[1];
		  //address=0;
		//  for(int e=0;)
		  //address = newscope*10+offset;
//				   push(*used_type1,(void*)&currentcommand->paramlist->list->int_val[0]);
				   push(*used_type1,(void*)&currentcommand->paramlist->list->int_val[1]);
//				   push(*used_type1,(int[]){ -420 });
				   vm_memory.used_stack[vm_memory.current_scope] += 3;

	   },
	   {/*pusha*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHA IN MACHINE",0);
					#endif

		  int overall;
		  overall = 0;
				   push(*used_type1,
				    (void*)&currentcommand->paramlist->list->int_val[0]);
				   vm_memory.used_stack[vm_memory.current_scope] += 1;

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

					in_loop_store();
				   vm_memory.used_stack[vm_memory.current_scope] -= 2;


	   },
	   {/*storeR*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN STORER IN MACHINE",0);
					#endif

				   store(pop(&used_type7,0),FLOAT,vm_memory.current_scope,(int*)pop(&used_type5,1));
				   vm_memory.used_stack[vm_memory.current_scope] -= 2;

	   },
	   {/*pushcI*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHCI IN MACHINE",0);
					#endif

				   push(*used_type1,(void*)&currentcommand->paramlist->list->int_val[0]);
				   vm_memory.used_stack[vm_memory.current_scope] += 1;

				   
	   },
	   {/*pushcR*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHCR IN MACHINE",0);
					#endif

				   push(*used_type1,(void*)&currentcommand->paramlist->list->int_val[0]);
				   vm_memory.used_stack[vm_memory.current_scope] += 1;

				   
	   },
	   {/*setrvI*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN SETRVI IN MACHINE",0);
					#endif
				   void * tempval;
				   tempval = NULL;
				   tempval = pop(&used_type6,3);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;
				   change_stack_value(used_type6, tempval,vm_memory.current_bp[vm_memory.current_scope]);
	   },
	   {/*setrvR*/

					#ifdef DEBUG
				   	dbprint(VMLIBC,"FOUND AN SETRVR IN MACHINE",0);
					#endif
		  			void * tempval;
		  			tempval = NULL;
		  			tempval = pop(&used_type6,3);
		  			vm_memory.used_stack[vm_memory.current_scope] -= 1;
		  			change_stack_value(used_type6, tempval,vm_memory.current_bp[vm_memory.current_scope]);

	   },
	   {/*returnf*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN RETURNF IN MACHINE",0);
					#endif

				   if(returnf(other_units,&c, &commandnum,&current_unit,&currentcommand)==1){
						#ifdef DEBUG
					  dbprint(VMLIBC,"Printing Memory",0);
					  showmemory();
					  dbprint(VMLIBC,"Printing Stack",0);
					  showstack();
					   #endif

					  continue;
				   }
	   },
	   {/*return*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN RETURN IN MACHINE",0);
					#endif

	   },
	   {/*ltI*/
					
					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN LTI IN MACHINE",0);
					#endif
				   operate('<',INT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;

				   
	   },
	   {/*ltR*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN LTR IN MACHINE",0);
					#endif
				   operate('<',FLOAT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;

	   },
	   {/* leI */

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN LEI IN MACHINE",0);
					#endif
				   relationship("<=",INT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;

	   },
	   {/* leR */

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN LER IN MACHINE",0);
					#endif

				   relationship("<=",FLOAT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;

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
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;
	   },
	   {/*mulI*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN MULI IN MACHINE",0);
					#endif

				   operate('*',INT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;
	   },
	   {/*mulR*/

					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN MULR IN MACHINE",0);
					#endif
				   operate('*',FLOAT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;
	   },
	   {/*addI*/

				#ifdef DEBUG
			   dbprint(VMLIBC,"FOUND AN ADDI IN MACHINE",0);
				#endif
				   operate('+',INT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;
	   },
	   { /*addR*/

				#ifdef DEBUG
			   dbprint(VMLIBC,"FOUND AN ADDR IN MACHINE",0);
				#endif
				   operate('+',FLOAT);
				   vm_memory.used_stack[vm_memory.current_scope] -= 1;
	   })
	   commandnum += 1;
	   	currentcommand = currentcommand->nextcommand;
	   #ifdef DEBUG
	   dbprint(VMLIBC,"Printing Memory",0);
	   showmemory();
	   dbprint(VMLIBC,"Printing Stack",0);
	   showstack();
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
    //	RETURN -- end
}

