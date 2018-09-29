#ifndef _MYCGMAIN_H
#define _MYCGMAIN_H
#include "typeint.h"

#include "debuglib.h"
#include <stdio.h>
#include <stdlib.h>
#include "dataint.h"
#define YYSTYPE data
#include "interpreter.y.h"
#include "interpreter.tab.h"
#include "Listint.h"
#include "commandint.h"
#include "interpreter.l.h"
#include "vm/vm.h"
#include "memlib.h"
#include <string.h>
#include <libgen.h>

extern FILE* yyin_temp;
extern FILE *yyin;
extern trans_u_list * All_T_Units;
const  char MAIN_STRING[]="main";



extern int Line_Number;
extern FILE *infile;
extern char* filename;
extern int globalcount;
extern boolcg founderror;
extern int labelcounter;
extern int offset_counter;
extern int othercounter;
extern int param_offset;
extern int mainlocal;
extern FILE *yyin;
extern int yyparse(void);
extern const char MAIN_STRING[];
#endif
