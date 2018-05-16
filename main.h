#ifndef _MYMAINH
#define _MYMAINH
#include "type.h"

extern int Line_Number;	
extern FILE *infile;
extern int globalcount;
Symtab *mysymtab;
int offset_counter;
int othercounter;
int param_offset;
int mainlocal;
bool founderror=FALSE;
extern int labelcounter;

#endif
