#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "memlib.h"
#include "debuglib.h"
#include "Listcg.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, const char ** argv){
	int * myint[6];
	char * tempCHAR;
	float *tempFLOAT[3];
	int mytag;
	int floattag, floattag2;
	int strtag;
	
	int_m *i_obj[5];
	float_m *f_obj[5];
	int_m *alpha, *beta, *charlie;
	str_m mystring_array[5];
	float_m *float_pointer;
	float_m st_float_pointer;
	
	//char *myname = "scott";
	
	int *objtag;
	init_mem_man();
	for(int i = 0; i < 5; i++){
		i_obj[i] = (int_m*)requestmemobj(1,INT);
		if( i_obj[i] != NULL){
			*i_obj[i]->value = (i+1) * 67;
		}
		f_obj[i] = (float_m*)requestmemobj(1,FLOAT);
		if( f_obj[i] != NULL ){
			*f_obj[i]->value = (i+1) * 67.5;
		}
		
	}
	mem_cleanup();
	return 0;	
}