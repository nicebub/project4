/*scott lorberbaum
 compilers spring 2004
	this is the main.c source file that calls the yylex() function in a loop, and outputs the token name and its semantic value.  The main also checks to make sure
that the file opened on the command line arguments ends in a '.c'.  Which also basically means that to send a file it just give the name after the ucc command:
		./ucc <filename_with.c> or
		./ucc < <filename_with.c>
*/
//#define MAINDEBUG

#if defined(DEBUGON) && defined(MAINDEBUG) && !defined(DEBUG)
//extern int yydebug = 1;
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(MAINDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "debuglib.h"
#include <stdio.h>
#include <stdlib.h>
#include "dataint.h"
#define YYSTYPE data
#include "interpreter.y.h"
#include "interpreter.tab.h"
#include "symtabint.h"
#include "Listint.h"
#include "commandint.h"
#include "main.h"
#include "transint.h"
#include "interpreter.l.h"
#include "vm/vm.h"
#include "memlib.h"
#include <string.h>
#include <libgen.h>
//extern int yytables_fload(FILE *);
//extern int yytables_destroy();
//extern int yylex_destroy();
//extern int yy_delete_buffer();
extern FILE* yyin_temp;
extern FILE *yyin;
extern trans_u_list * All_T_Units;
const  char MAIN_STRING[]="main";

int main(int argc, char **argv){
//	int offset_counter;
//	int othercounter;
//	int param_offset;
    int returnvalue;
	translation_unit * current_unit, *main_unit;


	init_debug("debuglib.c");
	init_trans_array();
	init_mem_man();
	initializelabel();



	founderror=FALSE;
//	othercounter=1;
	globalcount=0;
//	param_offset=0;
	infile = NULL;
	filename = NULL;
	filename = "main.c";
	All_T_Units = NULL;
	current_unit = NULL;
	main_unit = NULL;
	
	if((yyin_temp = checkargs(argc,argv)) == NULL){
		#ifdef DEBUG
			//filename = "input_filename.c";
			dbprint(MAINC,"Improper or no arguments given to compiler: Example filename -- ", 1, STR, filename);
		#endif
		return -1;
	}
	if(yyin_temp == NULL) return -1;
	yyin = yyin_temp;
    REQUESTMEM(filename, char, STR, *strlen(basename(argv[1]))+1)
//	filename = requestmem(strlen(argv[1])+1,STR, &tag);
	#ifdef DEBUG
    unsigned long *templen;
    templen = NULL;
    REQUESTMEM(templen,unsigned long, GENERIC, *sizeof(unsigned long))
    *templen = (unsigned long)strlen(argv[1]);
    dbprint(MAINC,"the strlen of argv[1] : ", 1,INT, *templen);
	#endif
	strlcpy(filename, basename(argv[1]), strlen(basename(argv[1]))+1);
	#ifdef DEBUG
    dbprint(MAINC,"the strlen of new filename : ", 2, STR, filename, INT, strlen(filename));
	#endif


	//offset_counter=5;

    do{
	#ifdef DEBUG
	    dbprint(MAINC,"STARTING TO PARSE MACHINE FILE",0);
	#endif
	yyparse();
	#ifdef DEBUG
	dbprint(MAINC,"PRINTING A TRANS LIST FROM PARSING",0);
	printTransList(All_T_Units);
	#endif

	}while(!feof(yyin));

	#ifdef DEBUG
	dbprint(MAINC,"Finished Parsing ",1, STR,filename);
	#endif
	if(infile !=NULL)
	    fclose(infile);
    infile=NULL;
    
	if(All_T_Units == NULL) return -1;
	main_unit = All_T_Units->list;
	while(strcmp(main_unit->name,"main") != 0 && main_unit->next_trans_unit != NULL){
		main_unit = main_unit->next_trans_unit;
	}
#ifdef DEBUG
    dbprint(MAINC,"now we have this translation unit: ",1,STR,main_unit->name);
    dbprint(MAINC,"TRYING OUT THE TRANSLATION UNIT ARRAY",0);
    for(int k = 0; k<translation_unit_amount;k++){
	   dbprint(MAINC,"translation unit name: ",1,STR,trans_array[k]->name);
	   printcommandList(trans_array[k]->commandlist);

    }
    dbprint(MAINC,"DONE WITH TRANSLATION UNIT ARRAY",0);
#endif
    
    returnvalue = run_virtual_machine(main_unit,trans_array,translation_unit_amount);
    
    filename=NULL;

	mem_cleanup();
	return returnvalue;
}
#define MAINFUNC
