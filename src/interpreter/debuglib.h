#ifndef _DEBUGLIB_H
#define _DEBUGLIB_H

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
	REGLIBC
};
static const char *which_strings[] = {
	":Lexer:", ":Parser", ":MEMMAN", "::Main:", "Command", "Translator", "::List:", ":Symtab", "Reglib.c"
};
void init_debug(void);

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
