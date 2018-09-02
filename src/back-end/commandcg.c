#ifdef DEBUGON
#ifdef COMMANDDEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif
#endif
#include "commandcg.h"
#include "memlib.h"
#include "debuglib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkargs(int argc, char** argv){
	if(argc >1){
		int in_size = strlen(argv[1]);

		#ifdef DEBUG
		dbprint(COMMANDC,"insize =: ", INT, &in_size);
		#endif

		if(in_size > 4 && strcmp(&argv[1][in_size-4], ".asm")==0){
			yyin = fopen(argv[1], "r");
			if(yyin == NULL){
				//fprintf(stderr,"%s: -> Cannot open input file %s\n", argv[0], argv[1]);
				error(3,"Cannot open input file ", argv[1]);
				return -1;
			}
			else {

				#ifdef DEBUG
					dbprint(COMMANDC,"Opening file for reading", STR, argv[1]);
				#endif

				return 0;
			}
		}
		else{
			error(COMMANDC, "Input file does not end in .asm", argv[1]);
			error(COMMANDC," 	(please supply the compiler with a correct .asm file)", argv[0]);
			return -1;
		}
	}
	else{
		error(COMMANDC,"	Compiler did not recieve an input file","");
		return -1;
	}

}

char* openfile(int argc, char** argv){
	char * tempstr, *tempstr2;
	int a;
	int tag;
//	tempstr = (char*) malloc(sizeof(char)*(strlen(argv[1])+3));  //add 3 for .s and null character
	infile=NULL;
	tempstr=NULL;
	tempstr2=NULL;
	tempstr = (char*) requestmem(strlen(argv[1])+3, STR, &tag);
	if(tempstr == NULL){ error(COMMANDC,"OUT OF MEMORY",""); exit(-1);}
	for(a=0;a<(strlen(argv[1])-4);a++)
		tempstr[a]=argv[1][a];
	tempstr[a] = '.';
	tempstr[a+1] = 's';
	tempstr[a+2] = '\0';
	if((infile = fopen(tempstr,"w"))==NULL){
		error(COMMANDC," Cannot open file for writing", tempstr);
		release(tempstr, STR, tag);
		tempstr=NULL;
//		free(tempstr);
		return NULL;
	}
	else{
		tempstr2 = (char*) requestmem(strlen(argv[1])+3, STR, &tag);
		if(tempstr2 == NULL){ error(COMMANDC,"OUT OF MEMORY",""); exit(-1);}
		strcpy(tempstr2, argv[1]);
		return tempstr2;
	}
}
FILE* openfilea(char *name){
	char * tempstr;
	FILE * outfile;
	int tag;
	int a;
	
	tempstr=NULL;
	outfile = NULL;
//	tempstr = (char*) malloc(sizeof(char)*strlen(name)+1);  //add 3 for .s and null character
	tempstr = (char*) requestmem(strlen(name)+1, STR, &tag);
	if(tempstr == NULL){ error(COMMANDC,"OUT OF MEMORY",""); exit(-1);}
	for(a=0;a<(strlen(name)+1);a++)
		tempstr[a]=name[a];
	tempstr[a] = '\0';
	if((outfile = fopen(tempstr,"w"))==NULL){
		error(COMMANDC," Cannot open file for writing", tempstr);
		release(tempstr, STR, tag);
		//free(tempstr);
		tempstr=NULL;
		return NULL;
	}
	else{
		return outfile;
	}
}
