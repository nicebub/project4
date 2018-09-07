#ifndef _MYCGMAIN_H
#define _MYCGMAIN_H
#include "typeint.h"

extern int Line_Number;
extern FILE *infile;
extern char* filename;
extern int globalcount;
extern boolcg founderror;
extern int labelcounter;
//Symtab *mysymtab;
extern int offset_counter;
extern int othercounter;
extern int param_offset;
extern int mainlocal;
extern FILE *yyin;
extern int yyparse(void);
extern const char MAIN_STRING[];
#endif
