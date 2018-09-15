#ifndef _MYCGL_H
#define _MYCGL_H

#include "dataint.h"

typedef struct _data YYSTYPE;
#define YYSTYPE_IS_DECLARED 1

//#include "interpreter.tab.h"
#ifndef MAX_STR_CONST
#define MAX_STR_CONST 50
#endif

extern int tag;
extern char string_buf[MAX_STR_CONST];
extern char *string_buf_ptr;
extern int warning(int, char*,char*);
extern int Line_Number;
extern YYSTYPE yylval;	

#endif
