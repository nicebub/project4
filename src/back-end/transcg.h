#ifndef _MYTRANSCG
#define _MYTRANSCG
#include <stdio.h>

typedef struct {
	int one;
	int two;
} cgPair;
int tag;
int globalcount;
int localcount;
int labelcounter;
extern FILE *infile;
extern int error(int which, char*, char*);

void initializelabel(void);


int getlabel(void); 	//return next available label number
void gen_label(char* name);	//generate label with label name : name
char* genlabelu(char* name, int labelnum); //generate a label with $<labelname><labelnumber>
void gen_instr(char* name); 	//for instructions without arguments
void gen_instr_I(char* name, int arg); // for instructions with arguments
void gen_instr_S(char *name, char* inS); 	//used for jump command
void gen_instr_F(char* name, float arg);	//use for float
void gen_call(char* funcname, int numargs);
void gen_instr_tI(char* name, int arg1, int arg2);

void gen_filename_comment();
void gen_section_text();
void gen_printf_dec();
void gen_global_main();
void gen_prolog_macro();
void gen_epilog_macro();
void gen_set_stack();
void gen_reverse_stack();

char* concat(char*, char*);	//concat 2 strings and return the answer, remember to free it when done
void nullout(char* name, int length);


#endif
