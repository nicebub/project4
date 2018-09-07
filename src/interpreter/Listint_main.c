#include "Listint.h"
#include "memlib.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char ** argv){

	int size,tag, int_array[2], int_array2[2];

	char *array[2] ,*array2[2], *inVal, *inVal2, *name, *name2;

	typecg type, type2;

	exprtype * expr, *expr2;

	List  *list1, *list8;

	ListC *list2, *list9;

	ListC *list3, *list10;

	ListP *list4, *list11;

	ListE *list5, *list12;

	commandList *list6, *list13;

	trans_u_list *list7, *list14;
	translation_unit * temp_unit;
	
	
	init_mem_man();

	
	list1 = list8 =  NULL;
	list2 = list9 =  NULL;
	list3 = list10 = NULL;
	list4 = list11 = NULL;
	list5 = list12 = NULL;
	list6 = list13 = NULL;
	list7 = list14 = NULL;

	temp_unit = NULL;

	expr = expr2 = NULL;

	inVal = requestmem(sizeof("hat")+1, STR, &tag);

	name = requestmem(sizeof("thomas")+1, STR, &tag);

	inVal = strcpy(inVal, "hat");

	name = strcpy(name, "thomas");

	array[0] = requestmem(sizeof(char)*2+1, STR, &tag);

	array[0] = strcpy(array[0], "ba");

	array[1] = NULL;

	int_array[0] = 559;

	int_array[1] = 6943;
	
	list1 = mklist("a");

	list2 = mklistC(array);

	list3 = mklistCi(int_array);

	type = INT;

	list4 = mklistP("bob", type);
	
	expr = (exprtype *)requestmem(sizeof(exprtype), GENERIC, &tag);

	expr->lval = FALSE;

	expr->type = STR;

	expr->numeric = FALSE;

	list5 = mklistE(expr);


	list6 = mkcommandList(inVal, list2);

	list7 = mkTransList(name, list6);
#if !defined DEBUG
	printListC(list2);
	printListC(list3);
	printListP(list4);
//	printListE(list5);
	printcommandList(list6);
	printTransList(list7);
#endif	
	inVal2 = requestmem(sizeof("hats")+1, STR, &tag);

	name2 = requestmem(sizeof("thomases")+1, STR, &tag);

	inVal2 = strcpy(inVal2, "hats");

	name2 = strcpy(name2, "thomases");

	array2[0] = requestmem(sizeof(char)*2+1, STR, &tag);

	array2[0] = strcpy(array2[0], "ot");

	array[1] = NULL;

	int_array[0] = 100001;

	int_array[1] = 4201;
	
	list8 = appendList(list1,"cater");

	list9 = appendListC(list3, array2[0], STR);

	list10 = appendListCi(list2, int_array2[0], INT);

	type2 = FLOAT;

	list11 = appendListP(list4, "todd", type2);
	
	expr2 = (exprtype *)requestmem(sizeof(exprtype), GENERIC, &tag);

	expr2->lval = TRUE;

	expr2->type = GENERIC;

	expr2->numeric = TRUE;

	list12 = appendListE(list5, expr2);
#ifdef DEBUG
	printListC(list9);
	printListC(list10);
	printListP(list11);
//	printListE(list5);
	printcommandList(list13);
	printTransList(list14);
#endif
	list13 = appendcommandList(list6, name2, list3);

	list14 = appendTransList(list7, name2, list13);

	list14 = prependTransList(list7, "mary", list13);

	temp_unit = getlastUnit(list14);

	list14 = concat_trans_unit_list(list14, list7);

#ifdef DEBUG
	printcommandList(list13);
	printTransList(list14);
#endif

	
//	deleteList(list1);
//	deleteListC(list2);
//	deleteListC(list3);
//	deleteListP(list4);
//	deleteListE(list5);
//	deletecommandList(list6);
//	deleteTransList(list7);

	mem_cleanup();

	
	list1 = NULL;
	list2 = NULL;
	list3 = NULL;
	list4 = NULL;
	list5 = NULL;
	list6 = NULL;
	list7 = NULL;
	array[0] = NULL;
	array[1] = NULL;
	expr = NULL;
	name = NULL;
	inVal = NULL;
	
	

	return 0;
	
}