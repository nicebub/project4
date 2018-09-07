/*scott lorberbaum
 compilers spring 2004
	this is the main.c source file that calls the yylex() function in a loop, and outputs the token name and its semantic value.  The main also checks to make sure
that the file opened on the command line arguments ends in a '.c'.  Which also basically means that to send a file it just give the name after the ucc command:
		./ucc <filename_with.c> or
		./ucc < <filename_with.c>
*/

#if defined(DEBUGON) && defined(MAINDEBUG) && !defined(DEBUG)
int yydebug = 0;
#define DEBUG
#endif
#include "debuglib.h"
#include <stdio.h>
#include <stdlib.h>
#include "dataint.h"
#define YYSTYPE data
#include "interpreter.tab.h"
#include "symtabint.h"
#include "Listint.h"
#include "commandint.h"
#include "main.h"
#include "transint.h"
#include "interpreter.l.h"
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
	int offset_counter;
	int othercounter;
	int param_offset;
	init_debug();
	translation_unit * current_unit, *main_unit;
	founderror=FALSE;
	othercounter=1;
	globalcount=0;
	param_offset=0;
	infile = NULL;
	filename = NULL;
	filename = "main.c";
	All_T_Units = NULL;
	current_unit = NULL;
	main_unit = NULL;
	initializelabel();
	
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
    printf("the size of argv[1] : %lu\n", strlen(argv[1]));
	strlcpy(filename, basename(argv[1]), strlen(basename(argv[1]))+1);
    printf("the size of new filename : %s %lu\n", filename, strlen(filename));

	init_mem_man();

/*	mysymtab = createTree(100);
	if(mysymtab == NULL){
//		free(filename);
		filename=NULL;
		filename = "main.c";
		error("Unable to construct symbol table\n","");
		return -1;
	}*/

	offset_counter=5;
/*	if(yytables_fload(openfilea("lex.yy.tables")) == 0){
		error("can't open lex.yy.tables","");
		return -1;
	}*/

	do{
	    printf("STARTING TO PARSE MACHINE FILE\n");

	yyparse();
	    
	printf("PRINTING A TRANS LIST FROM PARSING\n");

	printTransList(All_T_Units);

	}while(!feof(yyin));

	#ifdef DEBUG
	dbprint(MAINC,"Finished Parsing ",1, STR,filename);
	#endif
	if(infile !=NULL)
	    fclose(infile);
    infile=NULL;
    
/*	yytables_destroy();
	yylex_destroy();*/
//	free(filename);
    
	filename=NULL;
	if(All_T_Units == NULL) return -1;
	main_unit = All_T_Units->list;
	while(strcmp(main_unit->name,"main") != 0 && main_unit->next_trans_unit != NULL){
		main_unit = main_unit->next_trans_unit;
	}
	printf("now we have this translation unit: %s\n",main_unit->name);
    
	
	mem_cleanup();
	return 0;
}
#define MAINFUNC
