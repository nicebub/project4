#ifndef _DEBUGLIB_H
#define _DEBUGLIB_H
#include "typecg.h"
extern int Line_Number;
extern int current_char;
extern char * filename;
extern boolcg founderror;
int error(int which, char*,char*);
#ifdef DEBUG
#include "Listcg.h"
void dbprint(int which,char* s1, typecg intype, void *value);
void debugprint(int,char*,char*);
void debugprintt(int, trans_u_list*);
void debugprintd(int,char*, int);
void debugprintf(int,char*, float);
void debugprinta(int,char*, ListC *);
void debugprintc(int,char*, commandList *);
void debugprinta2(int,char*, char*, ListC *);
char *which_program(int);
#endif
#endif