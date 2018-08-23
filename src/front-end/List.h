#ifndef _MYLISTH
#define _MYLISTH
#include "type.h"
#include "expr.h"
typedef struct _listnodeE listnodeE;
struct _listnodeE{
	exprtype* expr;
	listnodeE *nextnode;
};

typedef struct {
	int listsize;
	listnodeE *list;
} ListE;


typedef struct _listnode listnode;
struct _listnode{
	char * val;
	listnode *nextnode;
};

typedef struct _listnodeP listnodeP;
struct _listnodeP{
	char * val;
	type ttype;
	listnodeP *nextnode;
};

typedef struct {
	listnode * list;
	int listsize;
	
} List;

typedef struct _ListP ListP;
struct _ListP{
	listnodeP * list;
	int listsize;
};

typedef struct {
    char * name;
    ListP * paramlist;
    type returntype;
    type ttype;
} funcheadertype;

void deleteList(List * inList);

List * mklist(char * inVal);

List * appendList(List * inList, char* inVal);

void deleteListP(ListP * inList);

ListP * mklistP(char* inName, type intype);

ListP * appendListP(ListP *inList, char* inName, type intype);

void deleteListE(ListE *inList);

ListE * mklistE(exprtype* expr);

ListE * appendListE(ListE * inList, exprtype* expr);

void printListP(ListP * inList);

#endif
