#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "commandcg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkargs(int argc, char** argv){
	if(argc >1){
		int in_size = strlen(argv[1]);
		#ifdef DEBUG
		fprintf(stderr,"%s: -> insize =: %d\n", argv[0], in_size);
		#endif
		if(in_size > 4 && strcmp(&argv[1][in_size-4], ".asm")==0){
			yyin = fopen(argv[1], "r");
			if(yyin == NULL){
				fprintf(stderr,"%s: -> Cannot open input file %s\n", argv[0], argv[1]);
				return -1;
			}
			else {
				#ifdef DEBUG
					fprintf(stderr, "%s: -> Opening file %s for reading\n", argv[0], argv[1]);
				#endif
				return 0;
			}
		}
		else{
			fprintf(stderr, "%s: -> Input file %s does not end in .asm\n", argv[0], argv[1]);
			fprintf(stderr, "%s: -> 	(please supply the compiler with a correct .asm file)\n", argv[0]);
			return -1;
		}
	}
	else{
		fprintf(stderr,"%s: -> Compiler did not recieve an input file\n", argv[0]);
		return -1;
	}

}

char* openfile(int argc, char** argv){
	char * tempstr;
	int a;
	tempstr = (char*) malloc(sizeof(char)*(strlen(argv[1])+3));  //add 3 for .s and null character
	for(a=0;a<(strlen(argv[1])-4);a++)
		tempstr[a]=argv[1][a];
	tempstr[a] = '.';
	tempstr[a+1] = 's';
	tempstr[a+2] = '\0';
	if((infile = fopen(tempstr,"w"))==NULL){
		fprintf(stderr, "%s: -> Cannot open file %s for writing\n", argv[0], tempstr);
		free(tempstr);
		return NULL;
	}
	else{
		return argv[1];
	}
}
