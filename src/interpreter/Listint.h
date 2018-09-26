#ifndef _MYLISTCG_H
#define _MYLISTCG_H
//#include "typeint.h"
#include "exprint.h"
#include "useful.def"
extern int tag;


struct command_st {
    command_names cmd_name;
    char* command_name_string;
};

extern struct command_st cmdinf[];


typedef struct _listnodeE listnodeE;
struct _listnodeE{
	exprtype* expr;
	listnodeE *nextnode;
};

typedef struct _listnodeC listnodeC;
struct _listnodeC{
	char * val[2];
	int	int_val[2];
    float float_val[2];
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
    enum _command_names name_enm;
	int length;
	ListC *paramlist;
	//struct ListCi *paramlisti;
	commandlisttype *nextcommand;
};

typedef struct _commandList commandList;
struct _commandList{
	int listsize;
	commandlisttype *list;
};

//typedef struct _transunit funcblock;
typedef struct _transunit translation_unit;
struct _transunit{
	char * name;
	commandList * commandlist;
	translation_unit * next_trans_unit;
//	typecg returntype;
//	typecg ttype;
};

typedef struct _trans_u_list trans_u_list;
struct _trans_u_list{
	int listsize;
	translation_unit * list;
};

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
void deleteListP(ListP * inList);
void deleteListE(ListE *inList);
void deletecommandList(commandList* inList);
void deleteTransList(trans_u_list* inList);

List * mklist(char * inVal);
ListC* mklistC(char * inVal[2],boolcg isfloat);
ListC* mklistCi(int inVal[2],boolcg isfloat);
ListP * mklistP(char* inName, typecg intype);
ListE * mklistE(exprtype* expr);
commandList* mkcommandList(char * inVal, ListC * inargs);
trans_u_list* mkTransList(char* inName, commandList* inList);

List * appendList(List * inList, char* inVal);
ListC* appendListC(ListC* inList, char* inVal, typecg inType);
ListC* appendListCi(ListC* inList, int inVal, typecg inType);
ListP * appendListP(ListP *inList, char* inName, typecg intype);
ListE * appendListE(ListE * inList, exprtype* expr);
commandList * appendcommandList(commandList * inList, char* inVal, ListC* inargs);
trans_u_list * appendTransList(trans_u_list* inTransList, char* inName, commandList* inList);
trans_u_list * prependTransList(trans_u_list* inTransList, char* inName, commandList* inList);
translation_unit * getlastUnit(trans_u_list* inTransList);
trans_u_list * concat_trans_unit_list(trans_u_list* , trans_u_list*);

void printListC(ListC * inList);
void printListC(ListC * inList);
void printListP(ListP * inList);
void printcommandList(commandList * inList);
void printTransList(trans_u_list *inList);


#endif
