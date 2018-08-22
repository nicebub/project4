#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "debuglib.h"
#include <stdio.h>
#include <stdlib.h>

int Line_Number = 1;
int current_char = 1;
char* filename = (char*) NULL;
#ifdef DEBUG
void debugprint(int which,char* s1, char*s2){
	char * program = NULL;
	switch(which){
		case 0:
				program = "Lexer:";
				break;
		case 1:
				program = "Parser";
				break;
				case 3:
				program =":Main:";
				break;
				
	}
	if(s1!=NULL && s2!=NULL){
			fprintf(stderr,"Debug::%s::%s:%d-> %s \"%s\"\n",program,filename,Line_Number,s1,s2);
	}
	else if(s1!=NULL && s2==NULL){
			fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,s1);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,"DEBUGPRINT HAS NOTHING TO PRINT");
}
void debugprintd(int which,char* s1, int s2){
	char * program = NULL;
	switch(which){
		case 0:
				program = "Lexer";
				break;
		default:
				program = "Parser";
	}
	if(s1!=NULL){
			fprintf(stderr,"Debug::%s::%s:%d-> %s \"%d\"\n",program,filename,Line_Number,s1, s2);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,"DEBUGPRINTD HAS NOTHING TO PRINT");
}
void debugprinta(int which,char* s1, ListC* inList){
	char * program = NULL;
	switch(which){
		case 0:
				program = "Lexer";
				break;
		default:
				program = "Parser";
	}
	if(s1!=NULL){
		fprintf(stderr,"Debug::%s::%s:%d-> %s \n",program,filename,Line_Number,s1);
			printListC(inList);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,"DEBUGPRINTA HAS NOTHING TO PRINT");
}

void debugprinta2(int which,char* s1, char* s2, ListC* inList){
	char * program = NULL;
	switch(which){
		case 0:
				program = "Lexer";
				break;
		default:
				program = "Parser";
	}
	if(s1!=NULL && s2 !=NULL){
		fprintf(stderr,"Debug::%s::%s:%d-> %s %s\n",program,filename,Line_Number,s1,s2);
			printListC(inList);
	}
	else if(s1 != NULL && s2 == NULL){
		fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,s1);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,"DEBUGPRINTA2 HAS NOTHING TO PRINT");
}
void debugprintc(int which,char* s1, commandList* inList){
	char * program = NULL;
	switch(which){
		case 0:
				program = "Lexer";
				break;
		default:
				program = "Parser";
	}
	if(s1!=NULL){
		fprintf(stderr,"Debug::%s::%s:%d-> %s \n",program,filename,Line_Number,s1);
			printcommandList(inList);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d-> %s\n",program,filename,Line_Number,"DEBUGPRINTC HAS NOTHING TO PRINT");
}

#endif