#ifndef _MYLISTH
#define _MYLISTH
#include "type.h"
#include "expr.h"

typedef struct {
	exprtype* expr;
	struct listnodeE *nextnode;
} listnodeE;

typedef struct {
	int listsize;
	listnodeE *list;
} ListE;

typedef struct {
	char * name;
	struct ListP *paramlist;
	type returntype;
	type ttype;
} funcheadertype;

typedef struct {
	char * val;
	struct listnode *nextnode;
} listnode;

typedef struct {
	char * val;
	type ttype;
	struct listnodeP *nextnode;
} listnodeP;

typedef struct {
	listnode * list;
	int listsize;
	
} List;

typedef struct {
	listnodeP * list;
	int listsize;
} ListP;

void deleteList(List * inList);

List * mklist(char * inVal);

List * appendList(List * inList, char* inVal);

void deleteListP(ListP * inList);

ListP * mklistP(char* inName, type intype);

ListP * appendListP(ListP *inList, char* inName, type intype);

void deleteListE(ListE *inList);

ListE * mklistE(exprtype* expr);

ListE * appendListE(ListE * inList, exprtype* expr);

#endif
