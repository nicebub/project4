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
boolcg founderror = FALSE;
char * which_program(int which){
	char * program;
	program = NULL;
	program = which_strings[which];
/*	switch(which){
		case 0:
				program = "Lexer:";
				break;
		case 1:
				program = "Parser";
				break;
				
		case 2:
				program = "MEMMAM";
				break;
				
		case 3:
				program =":Main:";
				break;
		default:
					program = "unknown";
					break;
	}*/
	return program;
}
int error(int which, char* s1, char* s2){
	char * program = NULL;
	program = which_program(which);
	fprintf(stderr,"ERROR::%s::%s:%d:%d-> %s \"%s\"\n",program,filename,Line_Number,current_char,s1,s2);
//	fprintf(stderr,"%s:%d:%d-> Error: %s %s\n",filename,Line_Number,current_char,s1,s2);
	founderror=TRUE;
    return 0;
}

#ifdef DEBUG
void debugprint(int which,const char* s1, const char*s2){
	char * program = NULL;
	program = which_program(which);
	if(s1!=NULL && s2!=NULL){
			fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%s\"\n",program,filename,Line_Number,current_char,s1,s2);
	}
	else if(s1!=NULL && s2==NULL){
			fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,s1);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,"DEBUGPRINT HAS NOTHING TO PRINT");
}
void debugprintd(int which,const char* s1, int s2){
	char * program = NULL;
	program = which_program(which);
	if(s1!=NULL){
			fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%d\"\n",program,filename,Line_Number,current_char,s1, s2);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,"DEBUGPRINTD HAS NOTHING TO PRINT");
}
void debugprinta(int which,const char* s1, ListC* inList){
	char * program = NULL;
	program = which_program(which);
	if(s1!=NULL){
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \n",program,filename,Line_Number,current_char,s1);
			printListC(inList);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,"DEBUGPRINTA HAS NOTHING TO PRINT");
}

void debugprinta2(int which,const char* s1, const char* s2, ListC* inList){
	char * program = NULL;
	program = which_program(which);
	if(s1!=NULL && s2 !=NULL){
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s %s\n",program,filename,Line_Number,current_char,s1,s2);
			printListC(inList);
	}
	else if(s1 != NULL && s2 == NULL){
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,s1);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,"DEBUGPRINTA2 HAS NOTHING TO PRINT");
}
void debugprintc(int which,const char* s1, commandList* inList){
	char * program = NULL;
	program = which_program(which);
	if(s1!=NULL){
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \n",program,filename,Line_Number,current_char,s1);
			printcommandList(inList);
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,"DEBUGPRINTC HAS NOTHING TO PRINT");
}
void debugprintt(int which,trans_u_list *inList){
	if(inList != NULL)
		printTransList(inList);
}
void dbprint(int which,const char * s1, typecg intype, void * value){
	char * program = NULL;
	if(value == NULL) return;
	program = which_program(which);
	switch(intype){
		case INT:
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%d\"\n",program,filename,Line_Number,current_char,s1, *(int*)value);
						break;
		case LONG:
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%lu\"\n",program,filename,Line_Number,current_char,s1, *(unsigned long*)value);
						break;
		case FLOAT:
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%.6f\"\n",program,filename,Line_Number,current_char,s1, *(float*)value);
						break;
		case CHAR:
		case STR:
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%s\"\n",program,filename,Line_Number,current_char,s1, (char*)value);
						break;
		default: 
						break;
	}
}


#endif