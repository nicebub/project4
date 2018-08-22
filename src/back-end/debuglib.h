#ifndef _DEBUGLIB_H
#define _DEBUGLIB_H
extern int Line_Number;
extern int current_char;
extern char * filename;
#ifdef DEBUG
#include "Listcg.h"
void debugprint(int,char*,char*);
void debugprintd(int,char*, int);
void debugprinta(int,char*, ListC *);
void debugprintc(int,char*, commandList *);
void debugprinta2(int,char*, char*, ListC *);

#endif
#endif