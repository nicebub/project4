#ifndef _MYCOMMANDCGH
#define _MYCOMMANDCGH
#include <stdio.h>

extern FILE *yyin;
FILE *infile;
extern char* filename;
int checkargs(int argc, char** argv);
char* openfile(int argc, char** argv);

#endif
