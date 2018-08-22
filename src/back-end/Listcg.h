#ifndef _MYLISTCGH
#define _MYLISTCGH
#include "typecg.h"
#include "exprcg.h"
typedef struct _listnodeE listnodeE;
struct _listnodeE{
	exprtype* expr;
	listnodeE *nextnode;
};

typedef struct _listnodeC listnodeC;
struct _listnodeC{
	char * val[2];
	int	int_val[2];
	typecg argtype[2];
	listnodeC *nextnode;
};

typedef struct {
	int listsize;
	listnodeE *list;
} ListE;

typedef struct _ListC ListC;
struct _ListC{
	listnodeC * list;
	int listsize;
};

typedef struct _commandlisttype commandlisttype;
struct _commandlisttype{
	char* name;
	int length;
	ListC *paramlist;
	//struct ListCi *paramlisti;
	commandlisttype *nextcommand;
};

typedef struct {
	int listsize;
	commandlisttype *list;
} commandList;

typedef struct {
	typecg argtype;
	int	intvalue;
	char*	stringvalue;
	int	length;
} commandargtype;


typedef struct {
	char * name;
	struct ListP *paramlist;
	typecg returntype;
	typecg ttype;
} funcheadertype;

typedef struct _listnode listnode;
struct _listnode{
	char * val;
	listnode *nextnode;
};

typedef struct _listnodeP listnodeP;
struct _listnodeP{
	char * val;
	typecg ttype;
	listnodeP *nextnode;
};

typedef struct {
	listnode * list;
	int listsize;
	
} List;

typedef struct {
	listnodeP * list;
	int listsize;
} ListP;

void deleteList(List * inList);
void deleteListC(ListC * inList);
void deleteCommandList(commandList* inList);

List * mklist(char * inVal);
commandList* mkcommandList(char * inVal, ListC * inargs);
ListC* mklistC(char * inVal[2]);
ListC* mklistCi(int inVal[2]);

List * appendList(List * inList, char* inVal);
commandList * appendcommandList(commandList * inList, char* inVal, ListC* inargs);
//commandList * appendcommandListi(commandList * inList, int* inVal, ListC* inargs);
ListC* appendListC(ListC* inList, char* inVal, typecg inType);
ListC* appendListCi(ListC* inList, int inVal, typecg inType);
void printListC(ListC * inList);
void printcommandList(commandList * inList);

void deleteListP(ListP * inList);
ListP * mklistP(char* inName, typecg intype);
ListP * appendListP(ListP *inList, char* inName, typecg intype);
void printListP(ListP * inList);
void printListC(ListC * inList);

void deleteListE(ListE *inList);
ListE * mklistE(exprtype* expr);
ListE * appendListE(ListE * inList, exprtype* expr);


#endif
