#ifndef _MEMLIB_H
#define _MEMLIB_H
#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "typecg.h"
#include "Listcg.h"
#define MAX_MEM 1000
typedef struct _int_memory_object int_m;
struct _int_memory_object {
	int *value;
	unsigned long address;
	int tag;
	typecg type;
};

typedef struct _float_memory_object float_m;
struct _float_memory_object {
	float *value;
	unsigned long address;
	int tag;
	typecg type;
};

typedef struct _str_memory_object str_m;
typedef struct _str_memory_object char_m;
struct _str_memory_object {
	char *value;
	unsigned long address;
	int tag;
	typecg type;
};
typedef struct _memory_manager mem_man;
struct _memory_manager{

	int inthash[MAX_MEM];
	int charhash[MAX_MEM];
	int floathash[MAX_MEM];
	int objhash[MAX_MEM];

	unsigned long int_address[MAX_MEM];
	unsigned long float_address[MAX_MEM];
	unsigned long char_address[MAX_MEM];
	unsigned long obj_address[MAX_MEM];

	int *intbucket[MAX_MEM];
	char *charbucket[MAX_MEM];
	float *floatbucket[MAX_MEM];
	void *objbucket[MAX_MEM];

	int intref[MAX_MEM];
	int charref[MAX_MEM];
	int floatref[MAX_MEM];
	int objref[MAX_MEM];

	int i_num;
	int cs_num;
	int f_num;
	int o_num;

	int i_amount;
	int cs_amount;
	int f_amount;
	int o_amount;
	
//	typecg itype[MAX_MEM];
//	typecg cstype[MAX_MEM];
//	typecg ftype[MAX_MEM];
	typecg otype[MAX_MEM];
	
};

mem_man mem_manager;

void init_mem_man();
int mem_cleanup();
void * requestmem(int size, typecg intype, int *tag);
void * requestmemobj(int size, typecg intype);
int get_avail_mem_tag(typecg intype);
int releaseall();
int releaseallobj();
int release(void * mem, typecg intype, int tag);
int releaseobj(void * obj, typecg intype);
void *copy_obj(void * obj1, typecg intype1);
void *assign_to_obj(void* obj1, typecg intype, void* obj2, typecg intype2);
#ifdef DEBUG
void list_i_mem();
void list_f_mem();
void list_cs_mem();
void list_o_mem();
void list_mem(typecg intype);
#endif
#endif