#ifndef _MYMAINH
#define _MYMAINH
#include "type.h"

extern int Line_Number;
extern FILE *infile;
extern char* filename;
extern int globalcount;
Symtab *mysymtab;
extern int offset_counter;
int othercounter;
int param_offset;
int mainlocal;
extern bool founderror;
extern int labelcounter;

#endif
