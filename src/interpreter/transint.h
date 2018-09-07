#ifndef _MYTRANSCG
#define _MYTRANSCG
#include <stdio.h>
#include "Listint.h"

typedef struct {
	int one;
	int two;
} cgPair;
extern int tag;
extern int globalcount;
extern int localcount;
extern int labelcounter;
extern int currentsp;
extern int local_total;
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
void gen_command(commandlisttype * inList);
void gen_run_commands(commandList* inList);

void gen_filename_comment(void);
void gen_section_text(void);
void gen_printf_dec(void);
void gen_global_main(void);
void gen_prolog_macro(void);
void gen_epilog_macro(void);
void gen_set_stack(void);
void gen_reverse_stack(void);
void gen_end_prog(void);
void gen_func_prolog(void);
void gen_func_epilog(void);
void check_and_gen_if_main(char * inName);


char * cut_str(char* inchar);

char* concat(char*, char*);	//concat 2 strings and return the answer, remember to free it when done
void nullout(char* name, int length);


#endif
