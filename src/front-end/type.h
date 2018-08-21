#ifndef _MYTYPEH
#define _MYTYPEH
typedef enum {
	PLS,
	MIN
} addtype;

typedef enum {
	DIV,
	MULT
} multype;

typedef enum {
	NEQ,
	EQEQ
} eqtype;

typedef enum {
	LES,
	LEQ,
	GRE,
	GEQ
} reltype;

typedef enum {
	FUNC,
	VAR,
	PARAM
} btype;

typedef enum {
	INT,
	FLOAT,
	VOID,
	CHAR,
	STR,
	REFINT,
	REFFLOAT,
	REFSTR
} type;

typedef enum {
	TRUE,
	FALSE
} bool;

typedef struct {
	type returntype;
	int num_param;
	type* param_type;
	bool bodydef;
	int label;
	int localcount;
	int actual_num;
} Funcb;

typedef struct {
	type type;
	int offset;
} Varb;

typedef struct {
	type type;
	int offset;
} Paramb;

typedef struct {
	char * name;
	void * binding;
	btype self;
} Entry;

typedef struct {
	Entry ** Stack; //Stack of Binary Search Trees
	int actualStacksize; //used to keep size and top of stack
	int Stacksize; //default of 100
} Symtab;

#endif
