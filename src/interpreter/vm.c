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
#include "memlib.h"
#include "debuglib.h"
#include "Listint.h"
#include "interpreter.y.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typecg *used_type1;
typecg *used_type2;
typecg *used_type3;
typecg used_type5, used_type6, used_type7;
//used_type4 = LONG;


#define PUSH(intype, cgintyp, value)  \
vm_stack.stacksize += 1; \
vm_stack.top[vm_stack.stacksize-1] = NULL; \
SWITCH(cgintyp,	\
REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], int, cgintyp), \
REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], float, cgintyp), \
REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], char, cgintyp, \
*strlen((char*)value)+1), \
REQUESTMEM(vm_stack.top[vm_stack.stacksize-1], intype, GENERIC)) \
temp = vm_stack.top[vm_stack.stacksize-1]; \
if(cgintyp == STR || cgintyp == CHAR){ \
char* temc=(char*)temp;\
	for(int g=0;g<strlen(value);g++)\
		temc[g] = ' ';\
	temc[strlen(value)] = '\0'; \
	strlcpy((char*)temp, (char*)value, strlen(temp)+1); \
/*	dbprint(VMLIBC,"3 ways of the string: ",3, STR, temp, STR, temc, STR, vm_stack.top[vm_stack.stacksize-1]);*/\
}\
else \
	*(intype*)temp =  *(intype*)value; \
vm_stack.type[vm_stack.stacksize-1] = cgintyp;


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



stack vm_stack;
memory vm_memory;

void initialize_machine(void){
    used_type1 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type1 = INT;
    used_type2 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type2 = FLOAT;
    used_type3 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type3 = STR;
    vm_stack.stacksize=0;
    vm_memory.current_scope= 0;
    vm_memory.total_scopes = 1;
    for(int i=0,j=0;i<STACKSIZE && j<MEMORY_SIZE;i++,j++){
		vm_stack.top[i]= NULL;
		vm_stack.type[i]=GENERIC;
		vm_memory.tag[j] = -1;
		vm_memory.address[j] = 0;
		vm_memory.offset[j] = -1;
		vm_memory.leveldiff[j] = -1;
	   for(int k = 0; k<SCOPE_SIZE;k++){
	   	vm_memory.yourmem[k][j] = NULL;
	     vm_memory.set[k][j] = 0;
	     vm_memory.type[k][j] = GENERIC;
	   }
	}
    for(int k=0;k<SCOPE_SIZE;k++){
	   	vm_memory.current_set_offset[k] = 0;
    }
}

void push(typecg inType, void* value, ...){
	void *temp;
	temp = NULL;
	if(value != NULL && vm_stack.stacksize < STACKSIZE){
		SWITCH(inType,PUSH(int, INT, value),\
			PUSH(float, FLOAT, value),\
			PUSH(char, STR, value),\
			PUSH(unsigned long, LONG, value))
	}
}

void* pop(typecg *intype, int number){
	if(number == -1 || vm_stack.stacksize == 0){
		return NULL;
	}
	void* temp = vm_stack.top[vm_stack.stacksize-1];
	vm_stack.top[vm_stack.stacksize-1] = NULL;
	*intype = vm_stack.type[vm_stack.stacksize-1];
	vm_stack.stacksize -= 1;
	return temp;
}

void showstack(){
	dbprint(VMLIBC,"-------------------------------------", 0);
	dbprint(VMLIBC, "---stacksize----", 1, INT, vm_stack.stacksize);
	for(int i=vm_stack.stacksize-1; i >= 0;i--){
		dbprint(VMLIBC, "---showstack iteration----", 1, INT, i, STR, "-----");
		dbprint(VMLIBC,"--",1, STR,(void*)typecg_strings[vm_stack.type[i]]);
		SWITCH(vm_stack.type[i], dbprint(VMLIBC,"--",1, INT,*(int*) vm_stack.top[i]), \
			dbprint(VMLIBC,"--",1, FLOAT,*(float*)vm_stack.top[i]), \
			dbprint(VMLIBC,"--",1, STR,(void*)vm_stack.top[i]), \
			dbprint(VMLIBC,"--",1, LONG,(unsigned long*)vm_stack.top[i]))
			dbprint(VMLIBC,"-------------------------------------",0);
	}
}

void showmemory(){
    dbprint(VMLIBC,"-------------------------------------", 0);
    dbprint(VMLIBC, "---memory----", 0);
    for(int i=0;i<vm_memory.total_scopes; i++){
	   dbprint(VMLIBC, "---showmemory scope ----", 1, INT, i);
	   for(int j=0;j<=vm_memory.current_set_offset[i];j++){
		  if(vm_memory.yourmem[i][j] != NULL){
		  	dbprint(VMLIBC,"--- showmemory allocated spot", 1, INT, j);
		  	dbprint(VMLIBC,"--",1, STR,(void*)typecg_strings[vm_memory.type[i][j]]);
			 SWITCH(vm_memory.type[i][j],
				   dbprint(VMLIBC,"--",1, INT,*(int*) vm_memory.yourmem[i][j]), \
				   dbprint(VMLIBC,"--",1, FLOAT,*(float*)vm_memory.yourmem[i][j]), \
				   dbprint(VMLIBC,"--",1, STR,(void*)vm_memory.yourmem[i][j]), \
				   dbprint(VMLIBC,"--",1, LONG,(unsigned long*)vm_memory.yourmem[i][j]))
			 dbprint(VMLIBC,"-------------------------------------",0);
		  }
		  else{
			 dbprint(VMLIBC,"--empty at location ",1, INT, j);

		  }
	   }
    }
}


void run_virtual_machine(translation_unit *main_unit,translation_unit **other_units,int unit_count){
    translation_unit * current_unit, *last_unit;
    commandlisttype *currentcommand, *last_unitlastcommand;
    int commandnum, current_frame_size;
    int c;
    current_unit=other_units[0];
    c=0;
    commandnum = 1;
    current_frame_size = 1;
    commandnum=current_frame_size;

    for(int b=0; b< MAX_FUNCTIONS && strncmp(current_unit->name,"$main",5) != 0 \
	   && current_unit->next_trans_unit != NULL;b++){
	   current_unit = other_units[b];
	   c=b;
    }
	#ifdef DEBUG
    dbprint(VMLIBC,"now we have this translation unit: ",1, STR, current_unit->name);
	#endif

    last_unit = main_unit;
    last_unitlastcommand= main_unit->commandlist->list->nextcommand->nextcommand->nextcommand;
   // used_type1 = INT;
    initialize_machine();
    vm_memory.current_scope =1;
    vm_memory.total_scopes +=1;

#ifdef DEBUG
    dbprint(VMLIBC,"Before use, here are the used_type1 and 3 ",
		  2, STR, typecg_strings[*used_type1], STR, typecg_strings[*used_type3]);
#endif
    push(*used_type1, (int[]) { 4 });
 //   used_type3 = STR;
    push(*used_type3, last_unit->name);
    //		SET SCOPE LEVEL 0
    //	   GETMEMORY_FOR_GLOBALS(main_unit->commandlist->list->nextcommand->paramlist->list->int_val[0]);
    // 		GET_first_function()
    //		GET_first_command()
    currentcommand = current_unit->commandlist->list;
    while(1){
#ifdef DEBUG
	   dbprint(VMLIBC,"What command are we going to run ", 1 , STR, currentcommand->name);
	   dbprint(VMLIBC,"The command has enumeration type ", 1 , STR, command_name_strings[currentcommand->name_enm]);
#endif
	   SWITCH_CMD(currentcommand->name_enm,
			    { /*alloc*/
					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN ALLOC IN MACHINE",0);
					#endif
			    },
			    {/*enter*/
					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN ENTER IN MACHINE",0);
					#endif

				   //enter(currentcommand->paramlist->list->int_val[0]);
			    },
			    {/*pushs*/
					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHS IN MACHINE",0);
					#endif
				   //used_type3 = STR;
				   push(*used_type3,(void*)currentcommand->paramlist->list->val[0]);

			    },
			    {/*call*/
					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN CALL IN MACHINE",0);
					#endif
				   if(strcmp(currentcommand->paramlist->list->val[0],"$scanf")==0 || \
					strcmp(currentcommand->paramlist->list->val[0],"$printf")== 0)
				   		call(currentcommand->paramlist->list->val[0],
						  currentcommand->paramlist->list->int_val[1]);
				   else{
					  int d;
					  for(d=0;d<MAX_FUNCTIONS && other_units[d] != NULL;d++){
						 if(strcmp(currentcommand->paramlist->list->val[0],other_units[d]->name)==0){
							break;
						 }
					  }
					  if(other_units[d] != NULL){
						 void * tempvalue;
						 int * in_off;
						 typecg intype;
//						 void * stack_top;
						 in_off = NULL;
						 tempvalue = NULL;
						 REQUESTMEM(in_off,int,INT)
//						 stack_top = NULL;
//						 stack_top = vm_stack.top[vm_stack.stacksize-1];
						 for(int g=0;g<currentcommand->paramlist->list->int_val[1];g++){
							tempvalue = vm_stack.top[(vm_stack.stacksize-1)-g];
							intype = vm_stack.type[(vm_stack.stacksize-1)-g];
#ifdef DEBUG
							dbprint(VMLIBC,"looking at type in call instruction ",1, STR, typecg_strings[intype]);
#endif
							*in_off = vm_memory.current_set_offset[vm_memory.current_scope+1] +5;
							store(tempvalue, intype,vm_memory.current_scope+1,in_off);
							*in_off = *in_off +1;
						 }
						 push(INT,(int[]) { commandnum });
						 push(STR,current_unit->name);
						 current_unit = other_units[d];
						 currentcommand = current_unit->commandlist->list;
						 commandnum = 1;
						 c =d;
						 vm_memory.current_scope +=1;
						 vm_memory.total_scopes +=1;
					  }
					  
					 // call_other(currentcommand->paramlist->list->val[0],
						//	   currentcommand->paramlist->list->int_val[1]);
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
			    },
			    {/*pusha*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHA IN MACHINE",0);
#endif
				   //used_type1 = INT;
				   push(*used_type1,(void*)&currentcommand->paramlist->list->int_val[0]);

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
				   int * tpop1;
				   int * tpop2;
				   tpop1 = NULL;
				   tpop2 = NULL;
				   tpop1 = pop(&used_type5,0);
				   tpop2 = pop(&used_type6,1);
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN STOREI IN MACHINE",0);
				   dbprint(VMLIBC,"first to be popped",1, INT, tpop1);
				   dbprint(VMLIBC,"second to be popped",1 ,INT, tpop2);
#endif
				   store(tpop1,INT,vm_memory.current_scope,(int*)tpop2);
				   
			    },
			    {/*storeR*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN STORER IN MACHINE",0);
#endif
				   store(pop(&used_type7,0),FLOAT,vm_memory.current_scope,(int*)pop(&used_type5,1));
				   
			    },
			    {/*pushcI*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHCI IN MACHINE",0);
#endif
				   //used_type1 = INT;
				   push(*used_type1,(void*)&currentcommand->paramlist->list->int_val[0]);
				   
			    },
			    {/*pushcR*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN PUSHCR IN MACHINE",0);
#endif
				   //used_type1 = INT;
				   push(*used_type1,(void*)&currentcommand->paramlist->list->int_val[0]);
				   
			    },
			    {/*setrvI*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN SETRVI IN MACHINE",0);
#endif

			    },
			    {/*setrvR*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN SETRVR IN MACHINE",0);
#endif

			    },
			    {/*returnf*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN RETURNF IN MACHINE",0);
#endif
					  char * unit_name;
					  int * cmdnum;
					  unit_name = NULL;
					  cmdnum = NULL;
					  int *returnval;
					  returnval =  (int*) pop(&used_type5,3);
					  unit_name =  (char*)pop(&used_type6,1);
					  cmdnum = 	(int*) pop(&used_type7,2);
					  int e;
					  for(e=0;e<MAX_FUNCTIONS && other_units[e] != NULL;e++){
						 if(strcmp(other_units[e]->name,unit_name) == 0){
							break;
						 }
					  }
				    	if(e < MAX_FUNCTIONS && other_units[e] != NULL){
						  	c=e;
				   			commandnum = *cmdnum;
				   			current_unit = other_units[e];
					    		currentcommand= current_unit->commandlist->list;
				   			for(int j=1;j<=commandnum;j++)
							    currentcommand = currentcommand->nextcommand;
				   			vm_memory.current_scope -=1;
				   			vm_memory.total_scopes -=1;

						  continue;
					   }
					   else{
						  printf("error not happening\n");
						  exit(-1);
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

			    },
			    {/*ltR*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN LTR IN MACHINE",0);
#endif

			    },
			    {/* leI */
					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN LEI IN MACHINE",0);
					#endif
				   

			    },
			    {/* leR */
					#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN LER IN MACHINE",0);
					#endif

			    },
			    {/*jump*/
#ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN JUMP IN MACHINE",0);
#endif

			    },
			    {/*jumpz*/
				   #ifdef DEBUG
				   dbprint(VMLIBC,"FOUND AN JUMPZ IN MACHINE",0);
				   #endif
				   int * conditional;
				   conditional = NULL;
				   conditional = pop(&used_type5,1);
				   if(*conditional == 0){
					  int k;
					  for(k=0;k< MAX_FUNCTIONS && other_units[k] != NULL;k++){
						 if(strcmp(currentcommand->paramlist->list->val[0],other_units[k]->name)==0){
							current_unit = other_units[k];
							currentcommand = current_unit->commandlist->list;
							commandnum = 1;
							c=k;
							break;
						 }
					  }
					  if(k < MAX_FUNCTIONS && other_units[k] !=NULL){
						 continue;
					  }
				   }
				   
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
		  	for(int i=count;i>0;i = i-2){
//			    infmt = va_arg(arglist,char*);
//		  		invalue = va_arg(arglist, void*);
			    invalue = pop(&used_type5,3);
			    infmt =   pop(&used_type6,4);
			    
			    REQUESTMEM(newval,char,STR, *MAX_STR_CONST)
			    scanf(infmt,newval);
#ifdef DEBUG
			    dbprint(VMLIBC,"newvalue ", 1, INT, *(int*)newval);
			    dbprint(VMLIBC,"invalue: ", 1, INT, *(int*)invalue);
#endif
			    store((int*)newval,INT,vm_memory.current_scope,(int*)invalue);
//			    vm_memory.yourmem[vm_memory.current_scope][*((int*)invalue)-5]);
			}
	   }
//	   va_end(arglist);

    }
    else if(strcmp(inname,"$printf") == 0){
		#ifdef DEBUG
	   dbprint(VMLIBC,"THIS IS A PRINTF",0);
		#endif
//	   va_list arglist;
//	   va_start(arglist,count);
	   if(count > 0){
		  char * infmt;
		  void * invalue;
		  infmt = NULL;
		  invalue = NULL;
		  for(int i=count;i>0;i = i-2){
			 //			    infmt = va_arg(arglist,char*);
			 //		  		invalue = va_arg(arglist, void*);
			 invalue = pop(&used_type5,3);
			 infmt =   pop(&used_type6,4);
//			 SWITCH()
			 printf(infmt, *(int*)invalue);
		  }
	   }
//	   va_end(arglist);

    }
}

void allocate(int inscope){
    
}

void fetch(typecg intype){
    int * first;
    int *second;
    int *third;
    first = NULL;
    second = NULL;
    third = NULL;
    first = (int*) pop(&used_type5, 1);
#ifdef DEBUG
    dbprint(VMLIBC,"WHAT IS THE FIRST ELEMENT POPPED ",1, INT, first);
#endif
    if(*first == -420){
	   switch(intype){
		  case INT:
			 second = pop(&used_type6,2);
			 third = pop(&used_type7,4);
			 push(intype, vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#ifdef DEBUG
			 dbprint(VMLIBC,"memory in fetch: ", 1, INT, *(int*)vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#endif
			 	break;
		  case FLOAT:
			 second = pop(&used_type6,3);
			 third = pop(&used_type7,4);
			 push(intype, vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#ifdef DEBUG
			 dbprint(VMLIBC,"memory in fetch: ", 1, INT, *(int*)vm_memory.yourmem[vm_memory.current_scope - (*third)][(*(int*)second)-5]);
#endif
			 	break;
		  default: break;
	   }
    }
    else{
	   push(intype, vm_memory.yourmem[vm_memory.current_scope][(*first)-5]);
#ifdef DEBUG
	   dbprint(VMLIBC,"memory in fetch: ", 1, INT, *(int*)vm_memory.yourmem[vm_memory.current_scope][(*(int*)first)-5]);
#endif
    }
}


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
		    printf("already set\n");
		    exit(-1);
		}
    }
    else{
	   printf("error with storing\n");
	   exit(-1);
    }
}

void enter(int inscope){
	#ifdef DEBUG
    dbprint(VMLIBC,"FOUND AN ENTER IN MACHINE",0);
    dbprint(VMLIBC,"CURRENT SCOPE: ",1,INT, vm_memory.current_scope);
	#endif

    if(inscope < SCOPE_SIZE){
	   if(inscope == 0){
		  vm_memory.current_scope = 1;
	   }
	   else{
		  if(inscope <= vm_memory.total_scopes){
			 vm_memory.current_scope = inscope;
		  }
		  else if(inscope == vm_memory.total_scopes +1){
			 vm_memory.current_scope = inscope;
			 vm_memory.total_scopes +=1;
		  }
	   }
    }
    else{
    }
	#ifdef DEBUG
    dbprint(VMLIBC,"CURRENT SCOPE after change: ",1,INT, vm_memory.current_scope);
	#endif

}

void *operate(char operator, typecg intype){
    int *tempin1;
    int *tempin2;
    float *tempfl1;
    float *tempfl2;
    tempin1 = NULL;
    tempin2 = NULL;
    tempfl1 = NULL;
    tempfl2 = NULL;
#define OPERATOR(x,op,y) x op y
#define SWITCH_OP(whichtemp,operator) switch(operator){\
						case '+':\
							*  whichtemp ## 1 =  OPERATOR((*  whichtemp ## 1), + ,( *  whichtemp ## 2)) ;\
    							break;\
						case '-':\
							*  whichtemp ## 1 =  OPERATOR((*  whichtemp ## 1), - ,( *  whichtemp ## 2)) ;\
							break;\
						case '*':\
							* whichtemp ## 1 =  OPERATOR((* whichtemp ## 1), * ,(* whichtemp ## 2)) ;\
    							break;\
						case '/':\
							if((* whichtemp ## 2) != 0){\
							* whichtemp ## 1 =  OPERATOR((* whichtemp ## 1), / ,(* whichtemp ## 2)) ;\
							break;\
							}\
							error(VMLIBC,"Cannot divide by 0","");\
							break; \
						case '<':\
							* whichtemp ## 1 =  OPERATOR((* whichtemp ## 1), < ,(* whichtemp ## 2)) ;\
							break;\
						case '>':\
							* whichtemp ## 1 =  OPERATOR((* whichtemp ## 1), > ,(* whichtemp ## 2)) ;\
							break;\
			 }\

    
#define SWITCH_OP2(intype,tempin, used_typnum,operator) {\
	tempin ## 1 = pop(&used_type ## used_typnum,1);\
	tempin ## 2 = pop(&used_type ## used_typnum,2);\
    	SWITCH_OP(tempin,operator)\
	push(intype,tempin ## 1);\
	return tempin ## 1;\
}
    
    switch(intype){
	   case INT:
		  SWITCH_OP2(INT,tempin,5,operator)
		  break;
	   case FLOAT:
		  SWITCH_OP2(FLOAT,tempfl,5,operator)
		  break;
	   default: 	return NULL;
    }
#undef OPERATOR
#undef SWITCH_OP
#undef SWITCH_OP2
}

void *relationship(char *relation, typecg intype){
    switch(intype){
	   case INT:
		  
		  return NULL;
	   case FLOAT:
		  return NULL;
	   default:
		  return NULL;
    }
}
