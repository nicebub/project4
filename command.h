#ifndef _MYCOMMANDH
#define _MYCOMMANDH
#include <stdio.h>

extern FILE *yyin;
FILE *infile;
int checkargs(int argc, char** argv);
int openfile(int argc, char** argv);

#endif
