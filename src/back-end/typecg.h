#ifndef _MYTYPECGH
#define _MYTYPECGH
typedef enum {
	PLS,
	MIN
} addtypecg;

typedef enum {
	DIV,
	MULT
} multypecg;

typedef enum {
	NEQ,
	EQEQ
} eqtypecg;

typedef enum {
	LES,
	LEQ,
	GRE,
	GEQ
} reltypecg;

typedef enum {
	FUNC,
	VAR,
	PARAM
} btypecg;

typedef enum {
	INT,
	FLOAT,
	VOID,
	CHAR,
	STR,
	REFINT,
	REFFLOAT,
	REFSTR,
	COMMAND,
	LONG,
	OBJ
} typecg;

typedef enum {
	FALSE,
	TRUE
} boolcg;

typedef struct {
	typecg returntype;
	int num_param;
	typecg* param_type;
	boolcg bodydef;
	int label;
	int localcount;
	int actual_num;
} Funcbcg;

typedef struct {
	typecg type;
} commandBinding;

typedef struct {
	typecg type;
	int offset;
} Varbcg;

typedef struct {
	typecg type;
	int offset;
} Parambcg;

typedef struct {
	char * name;
	void * binding;
	btypecg self;
} Entry;

typedef struct {
	Entry ** Stack; //Stack of Binary Search Trees
	int actualStacksize; //used to keep size and top of stack
	int Stacksize; //default of 100
} Symtab;

#endif
