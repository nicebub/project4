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
	
	const static char *myname = "scott";
	
	int *objtag;
	init_mem_man();
	for(int i = 0; i < 5; i++){
		if((i_obj[i] = (int_m*)requestmemobj(1,INT))!=NULL){
			*i_obj[i]->value = (i+1) * 67;
		}
		if((f_obj[i] = (float_m*)requestmemobj(1,FLOAT))!=NULL){
			*f_obj[i]->value = (i+1) * 67.5;
		}
		
	}
/*	releaseobj((void*)i_obj[3],INT);
	i_obj[3]=NULL;
	for(int i = 0; i < 5; i++){
		releaseobj((void*)i_obj[i], INT);
	}*/
	mem_cleanup();
	return 0;
	
/*	mytag = 2;
	for(int i = 0; i < 5; i++){
		i_obj[i] = (int_m*)requestmemobj(1,INT);
	}

	if(releaseobj((void*) i_obj[2], INT) == -1){
		#ifdef DEBUG
		dbprint(2,"Couldn't release INT OBJ" ,INT, &mytag);
		#endif
		;
	}
	i_obj[2]=NULL;
	
	alpha = (int_m*) requestmemobj(1, INT);
	if( alpha->tag != -1){
		*alpha->value = 50 * 400;
		alpha->address = (unsigned long)&alpha->value;
		#ifdef DEBUG
		dbprint(2,"ALPHA Tag Given: ", INT, (void*)&alpha->tag);
		dbprint(2,"ALPHA Value Given:", INT, (void*) alpha->value);
		dbprint(2,"ALPHA Address Equals:", LONG, (void*)alpha->address);
		#endif
	}
//	i_obj[2]= alpha;
	mytag = 4;
	if(releaseobj((void*) i_obj[4], INT) == -1){
		#ifdef DEBUG
		dbprint(2,"Couldn't release INT OBJ",STR,&mytag);
		#endif
		;
	}
	i_obj[4]=NULL;

	beta = (int_m*) requestmemobj(1, INT);
	if( beta->tag != -1){
		#ifdef DEBUG
		dbprint(2,"BETA Tag Given: ", INT, (void*)&beta->tag);
		#endif
		*beta->value = 50 * 400;
		beta->address = (unsigned long)&beta->value;
		#ifdef DEBUG
		dbprint(2,"BETA Value Given:", INT, (void*) beta->value);
		dbprint(2,"BETA Address Equals:", LONG, (void*)beta->address);
		#endif
	}
	charlie = (int_m*) copy_obj((void*)beta, INT);
	if(releaseobj((void*) charlie, INT) == -1){
		#ifdef DEBUG
		dbprint(2,"Couldn't release CHARLIE",INT,&charlie->tag);
		#endif
		;
	}
	charlie=NULL;
	
	for(int i = 0; i < mem_manager.i_num; i++){
		if(releaseobj((void*) i_obj[i], INT) == -1){
			#ifdef DEBUG
			dbprint(2,"Couldn't release INT OBJ",STR,"");
			#endif
			;
		}
	}
	releaseobj((void*)alpha, INT);
	releaseobj((void*)beta, INT);
	releaseobj((void*)charlie, INT);
	releaseall();*/
}