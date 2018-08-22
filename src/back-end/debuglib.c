#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "debuglib.h"
#include <stdio.h>
#include <stdlib.h>

int Line_Number = 1;
char* filename = (char*) NULL;
#ifdef DEBUG
void debugprint(int which,char* s1, char*s2){
	char * program = NULL;
	if(s1!=NULL && *s1 && s2!=NULL && *s2){
			switch(which){
				case 0:
						program = "Lexer:";
						break;
				case 1:
						program = "Parser";
						break;
						case 3:
						program ="Main";
						break;
						
			}
			fprintf(stderr,"Debug::%s::%s:%d-> %s \"%s\"\n",program,filename,Line_Number,s1,s2);
	}
	else if(s1!=NULL && *s1 && s2==NULL){
			fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,s1);
	}
}
void debugprintd(int which,char* s1, int s2){
	char * program = NULL;
	if(s1!=NULL && *s1){
		switch(which){
			case 0:
					program = "Lexer";
					break;
			default:
					program = "Parser";
		}
			fprintf(stderr,"Debug::%s::%s:%d-> %s \"%d\"\n",program,filename,Line_Number,s1, s2);
	}
}
void debugprinta(int which,char* s1, ListC* inList){
	char * program = NULL;
	if(s1!=NULL && *s1){
		switch(which){
			case 0:
					program = "Lexer";
					break;
			default:
					program = "Parser";
		}
		fprintf(stderr,"Debug::%s::%s:%d-> %s \n",program,filename,Line_Number,s1);
			printListC(inList);
	}
}

void debugprinta2(int which,char* s1, char* s2, ListC* inList){
	char * program = NULL;
	if(s1!=NULL && *s1 && s2 !=NULL){
		switch(which){
			case 0:
					program = "Lexer";
					break;
			default:
					program = "Parser";
		}
		fprintf(stderr,"Debug::%s::%s:%d-> %s %s\n",program,filename,Line_Number,s1,s2);
			printListC(inList);
	}
}
void debugprintc(int which,char* s1, commandList* inList){
	char * program = NULL;
	if(s1!=NULL && *s1){
		switch(which){
			case 0:
					program = "Lexer";
					break;
			default:
					program = "Parser";
		}
		fprintf(stderr,"Debug::%s::%s:%d-> %s \n",program,filename,Line_Number,s1);
			printcommandList(inList);
	}
}

#endif