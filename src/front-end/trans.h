#ifndef _MYTRANS
#define _MYTRANS
#include <stdio.h>

typedef struct {
	int one;
	int two;
} Pair;
int globalcount;
int localcount;
int labelcounter;
extern FILE *infile;

void initializelabel(void);


int getlabel(void); 	//return next available label number
void gen_label(char* name);	//generate label with label name : name
char* genlabelw(char* name, int labelnum); //generate a label with $<labelname><labelnumber>
void gen_instr(char* name); 	//for instructions without arguments
void gen_instr_I(char* name, int arg); // for instructions with arguments
void gen_instr_S(char *name, char* inS); 	//used for jump command
void gen_instr_F(char* name, float arg);	//use for float
void gen_call(char* funcname, int numargs);
void gen_instr_tI(char* name, int arg1, int arg2);

char* concat(char*, char*);	//concat 2 strings and return the answer, remember to free it when done
void nullout(char* name, int length);


#endif
