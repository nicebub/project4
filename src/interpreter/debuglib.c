#if defined(DEBUGON) && !defined(DEBUG)
#define DEBUG
#endif

#include "debuglib.h"
//#include "memlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int Line_Number = 1;
int current_char = 1;
char *filename = (char*) NULL;
boolcg founderror = FALSE;

void init_debug(char * in_filename){
	Line_Number = 1;
	current_char = 1;
	filename = (char*) NULL;
    if(in_filename ==NULL){
	   in_filename = "debuglib.c";
	   filename = (char*) malloc(sizeof(char)*strlen("debuglib.c")+1);
    }
	else
	    filename = (char*) malloc(sizeof(char)*strlen(in_filename)+1);
    for(int j = 0; j<strlen(in_filename)+1;j++)
	   filename[j]=' ';
    filename[strlen(in_filename)] = '\0';
    strlcpy(filename, in_filename, strlen(filename)+1);
	founderror = FALSE;
}
char * which_program(int which){
	char * program;
	program = NULL;
	program = (char*)which_strings[which];
	return program;
}
int error(int which, char* s1, char* s2){
	char * program = NULL;
	program = which_program(which);
	fprintf(stderr,"ERROR::%s::%s:%d:%d-> %s \"%s\"\n",program,filename,Line_Number,current_char,s1,s2);
	founderror=TRUE;
    return 0;
}


int warning(int which, char *s1, char* s2){
	char * program = NULL;
	program = which_program(which);
	fprintf(stderr,"WARNING::%s::%s:%d:%d-> %s \"%s\"\n",program,filename,Line_Number,current_char,s1,s2);
    return 0;
}


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
		#ifdef LISTDEBUG
		printListC(inList);
		#endif
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,"DEBUGPRINTA HAS NOTHING TO PRINT");
}

void debugprinta2(int which,const char* s1, const char* s2, ListC* inList){
	char * program = NULL;
	program = which_program(which);
	if(s1!=NULL && s2 !=NULL){
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s %s\n",program,filename,Line_Number,current_char,s1,s2);
		#ifdef LISTDEBUG
		printListC(inList);
		#endif
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
		#ifdef LISTDEBUG
		printcommandList(inList);
		#endif
	}
	else
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,"DEBUGPRINTC HAS NOTHING TO PRINT");
}
void debugprintt(int which,trans_u_list *inList){
	if(inList != NULL){
		#ifdef LISTDEBUG		
		printTransList(inList);
		#endif
	}
}

/*

 void dbprint(int which,const char * s1, int count [, ypecg intype, void * value])

*/

void dbprint(int which,const char * s1,int count, ...){
	char * program = NULL;
	void * value = NULL;
	int  intvalue;
	double floatvalue;
	void * charvalue;
	//intvalue = -1;
	//floatvalue =-1.1;
	charvalue = NULL;
	typecg intype;
    program = (char*)which_strings[which];
	if(count == 0){
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s\n",program,filename,Line_Number,current_char,s1);
		return;
	}
	va_list arglist;
	va_start(arglist,count);
	for(int i=count;i>0;i--){
		intype = va_arg(arglist,typecg);
		
/*		SWITCH(intype, \
		DBPRINT(intvalue,int,d),\
		DBPRINT(floatvalue,float,.6f),\
		DBPRINT(charvalue,char*,s),\
		DBPRINT(value,unsigned long*,lu))*/
		switch(intype){
			case INT:
						intvalue = va_arg(arglist,int);
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%d\"\n",program,filename,Line_Number,current_char,s1, intvalue);
						break;
			case FLOAT:
						floatvalue = va_arg(arglist,double);
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%.6f\"\n",program,filename,Line_Number,current_char,s1, floatvalue);
						break;
			case STR:
			case CHAR:
						charvalue = va_arg(arglist,char*);
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%s\"\n",program,filename,Line_Number,current_char,s1, charvalue);
						break;
			case REFINT:
			case REFSTR:
			case REFFLOAT:
			case LONG:
						value = va_arg(arglist,unsigned long*);
						fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \"%#lx\"\n",program,filename,Line_Number,current_char,s1, *(unsigned long*)value);
						break;
			default:
			break;
		}
		
	}
	va_end(arglist);
}
