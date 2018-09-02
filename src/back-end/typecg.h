#ifndef _MYTYPECGH
#define _MYTYPECGH
typedef enum {
	PLS,
	MIN
} addtypecg;
static const char *addtypecg_strings[] = {
	"PLUS", "MINUS"
};


typedef enum {
	DIV,
	MULT
} multypecg;
static const char *multtypecg_strings[] = {
	"DIV", "MULT"
};


typedef enum {
	NEQ,
	EQEQ
} eqtypecg;
static const char *eqtypecg_strings[] = {
	"NOT EQUAL", "DOUBLE EQUALS"
};

typedef enum {
	LES,
	LEQ,
	GRE,
	GEQ
} reltypecg;
static const char *reltypecg_strings[] = {
	"LESS THAN", "LESS THAN OR EQUAL", "GREATER THAN", "GREATER THAN OR EQUAL"
};

typedef enum {
	FUNC,
	VAR,
	PARAM
} btypecg;
static const char *btypecg_strings[] = {
	"FUNCTION", "VARIABLE", "PARAMETER"
};

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
	OBJ,
	GENERIC
} typecg;
static const char *typecg_strings[] = {
	"INT", "FLOAT", "VOID", "CHAR", "STR", "REFINT", "REFFLOAT", "REFSTR", "COMMAND", "LONG", "OBJ", "GENERIC"
};

typedef enum {
	FALSE,
	TRUE
} boolcg;
static const char *boolcg_strings[] = {
	"FALSE", "TRUE"
};

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
