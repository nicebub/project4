#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkargs(int argc, char** argv){
	if(argc >1){
		if(argv[1][strlen(argv[1])-1] == 'c'){
			if(argv[1][strlen(argv[1])-2] == '.'){
				yyin = fopen(argv[1], "r");
				if(yyin == NULL){
					fprintf(stderr,"%s: cannot open input file\n", argv[1]);
					return -1;
				}
				else return 0;
			}
			else{
				fprintf(stderr, "%s: does not end in .c\n", argv[1]);
				return -1;
			}
		}
		else{
			fprintf(stderr, "%s: does not end in .c\n", argv[1]);
			return -1;
		}
	}
	else{
		fprintf(stderr,"did not recieve an input file\n");
		return -1;
	}

}

char* openfile(int argc, char** argv){
	char * tempstr;
	int a;
	tempstr = (char*) malloc(sizeof(char)*(strlen(argv[1])+4));  //changed to 4 because losing a '\0' was 5
	for(a=0;a<(strlen(argv[1])-2);a++)
		tempstr[a]=argv[1][a];
	tempstr[a] = '.';
	tempstr[a+1] = 'a';
	tempstr[a+2] = 's';
	tempstr[a+3] = 'm';
	tempstr[a+4] = '\0';
	if((infile = fopen(tempstr,"w"))==NULL){
		fprintf(stderr, "cannot open file %s for writing\n", tempstr);
		free(tempstr);
		return NULL;
	}
	else{
		free(tempstr);
		return argv[1];
	}
}
