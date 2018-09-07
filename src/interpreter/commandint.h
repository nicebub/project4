#ifndef _MYCOMMANDCGH
#define _MYCOMMANDCGH
#include <stdio.h>

//extern FILE *yyin;
extern FILE *infile;
extern char* filename;
FILE* checkargs(int argc, char** argv);
char* openfile(int argc, char** argv);
FILE* openfilea(char* name);
extern int tag;
extern FILE *yyin_temp;
#endif
