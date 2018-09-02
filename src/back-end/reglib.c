#include "reglib.h"
#include "memlib.h"
#include "debuglib.h"
//#include <stdio.h>
#include <stdlib.h>


void initialize_register_stack(){
	int tag;
	int register_count;
	reg * temp_reg;
	temp_reg = NULL;
	tag = 0;
	register_count = REGISTER64_COUNT;
	Register_Stack.top = NULL;
	Register_Stack.stacksize = 0;
	for(int i = 0; i< register_count; i++){
		temp_reg = (reg*) requestmem(sizeof(reg), GENERIC, &Register_Stack.tag[i]);
		temp_reg->next_register = Register_Stack.top;
		Register_Stack.top = temp_reg;
		Register_Stack.inthewild[i] = NULL;
#ifdef DEBUG
		dbprint(REGLIBC,"Adding Register ", STR, (void*)bit64_strings[i]);
#endif
		Register_Stack.top->reg_type = i;
		Register_Stack.stacksize += 1;
	}
}
void delete_register_stack(){
}
reg * get_next_register(){
	reg *temp_reg;
	bit64_bitreg type;
	temp_reg = NULL;
	if(registers_left() > 0 && Register_Stack.top != NULL){
		temp_reg = Register_Stack.top;
		type = temp_reg->reg_type;
		Register_Stack.top = Register_Stack.top->next_register;
		temp_reg->next_register = NULL;
		Register_Stack.stacksize -= 1;
		Register_Stack.inthewild[type] = temp_reg;
		return temp_reg;
	}
	else if(registers_left() == 0){
		error(REGLIBC,"No more registers left. Cannot allocate another register.","");
	}
	return NULL;
}
void release_register(reg* inRegister){
	reg *temp_reg;
	int type;
	int register_count;
	temp_reg = NULL;
	register_count = REGISTER64_COUNT;
	if(registers_left() == register_count){
		dbprint(REGLIBC,"Cannot release register, all registers already available",STR,"");
	}
	else if(inRegister != NULL){
		temp_reg = inRegister;
		temp_reg->next_register = Register_Stack.top;
		Register_Stack.top = temp_reg;
		Register_Stack.stacksize += 1;
		
	}
	
}
int registers_left(){
	return Register_Stack.stacksize;
}

#ifdef DEBUG
void print_register_stack(){
	reg * tempreg;
	tempreg = NULL;
	tempreg = Register_Stack.top;
	if(Register_Stack.top != NULL && Register_Stack.stacksize != 0){
		for(int i=Register_Stack.stacksize-1; i >= 0 && tempreg != NULL;i--){
				dbprint(REGLIBC,"Register ", STR, (void*)bit64_strings[tempreg->reg_type]);
				dbprint(REGLIBC,"in position ", INT, &i);
				tempreg = tempreg->next_register;
			
		}
	}
}
#endif