#ifndef _MYDATACGH
#define _MYDATACGH
#include "symtabcg.h"
#include "Listcg.h"
#include "transcg.h"

typedef struct {
	union {
		char cvalue;
		char *svalue;
		int ivalue;
		float fvalue;
		List *lstvalue;
		ListP *lstpvalue;
		ListC *cargvalue;
		typecg tvalue;
		addtypecg addopvalue;
		multypecg multopvalue;
		eqtypecg eqopvalue;
		ListE *lstevalue;
		//Pair pairvalue;
		exprtype *exprvalue;
		reltypecg relopvalue;
		commandlisttype *commandlistvalue;
		translation_unit * transunitvalue;
		//Entry *funcentvalue;
	} value;
	int one;
	int two;
	int mainlabel;
	typecg ttype;
	boolcg lval;
	commandList* command_list;
	commandBinding *command_binding;
	trans_u_list * trans_list;
	Entry *funcent;
	boolcg numeric;
	Entry* entry;
	Funcbcg* funcbinding;
	int params;
} data;


#endif
