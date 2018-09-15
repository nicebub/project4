#ifndef _DEBUGLIB_H
#define _DEBUGLIB_H

#define DBPRINT(in_value, intype, fmt) {\
		 fmt; \
		in_value = va_arg(arglist,intype); \
		fprintf(stderr,"Debug::%s::%s:%d:%d-> %s \" %fmt \"\n",program,filename,Line_Number,current_char,s1, in_value);} \

#define DBPRINT_HELPER(intype) \
			SWITCH(intype, \
			DBPRINT(intvalue,int,%d),\
			DBPRINT(floatvalue,float,%.6f),\
			DBPRINT(charvalue,char*,%s),\
			DBPRINT(value,unsigned long*,%lu))




#include "Listint.h"
#include "typeint.h"

extern int Line_Number;
extern int current_char;
extern char * filename;
extern boolcg founderror;

int error(int which, char*,char*);
int warning(int which, char*, char*);

typedef enum _which which;
enum _which{
	LEXER,
	PARSER,
	MEMLIBC,
	MAINC,
	COMMANDC,
	TRANSC,
	LISTC,
	SYMTABC,
	REGLIBC,
    	VMLIBC
};
static const char *which_strings[] = {
	":Lexer:", ":Parser", ":MEMMAN", "::Main:", "Command", "Translator", "::List:", ":Symtab", "Reglib.c", "vmlib.c"
};

void init_debug(char* in_filename);

/*extern void dbprint(int which,const char* s1, int count, typecg intype, void *value);*/
extern void dbprint(int which,const char* s1, int count, ...);
void debugprint(int,const char*,const char*);
void debugprintt(int, trans_u_list*);
void debugprintd(int,const char*, int);
void debugprintf(int,const char*, float);
void debugprinta(int,const char*, ListC *);
void debugprintc(int,const char*, commandList *);
void debugprinta2(int,const char*, const char*, ListC *);
char *which_program(int);
#endif
