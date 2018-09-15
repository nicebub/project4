#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "dataint.h"

//#define yyerrok

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

//#define YYSTYPE data

typedef data YYSTYPE;
#define YYSTYPE_IS_DECLARED 1
#ifndef MAX_FUNCTIONS
#define MAX_FUNCTIONS 30
#endif

#define ADD_TO_TRANS_ARRAY(inlist) if(translation_unit_amount >= MAX_FUNCTIONS){ \
					error(PARSER, "too many functions declared", ""); \
				} \
				else{ \
					trans_array[translation_unit_amount] = inlist->list; \
					translation_unit_amount += 1; }


extern int Line_Number;
extern boolcg founderror;
extern int globalcount;
extern int mainlocal;
extern int othercounter;
extern int param_offset;
extern int offset_counter;
//extern Symtab* mysymtab;

extern int mainlabel;
extern Funcbcg* currentFunc;
extern trans_u_list *All_T_Units;
extern translation_unit *trans_array[MAX_FUNCTIONS];
extern int translation_unit_amount;



extern int yyerror(const char *);
extern int yylex (void);
extern int warning(int, char*,char*);
extern int error(int which, char*,char*);
extern void init_trans_array(void);

#endif
