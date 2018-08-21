#ifndef _MYDATAH
#define _MYDATAH
#include "symtab.h"
#include "List.h"
#include "trans.h"

typedef struct {
	union {
		char cvalue;
		char *svalue;
		int ivalue;
		float fvalue;
		List *lstvalue;
		ListP *lstpvalue;
		type tvalue;
		addtype addopvalue;
		multype multopvalue;
		eqtype eqopvalue;
		ListE *lstevalue;
		Pair pairvalue;
		exprtype *exprvalue;
		reltype relopvalue;
		funcheadertype *funcheadervalue;
		//Entry *funcentvalue;
	} value;
	int one;
	int two;
	int mainlabel;
	char * name;
	type ttype;
	bool lval;
	Entry *funcent;
	bool numeric;
	Entry* entry;
	Funcb* funcbinding;
	int params;
} data;


#endif
