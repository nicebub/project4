/*scott lorberbaum
 compilers spring 2004
	this is the main.c source file that calls the yylex() function in a loop, and outputs the token name and its semantic value.  The main also checks to make sure
that the file opened on the command line arguments ends in a '.c'.  Which also basically means that to send a file it just give the name after the ucc command:
		./ucc <filename_with.c> or
		./ucc < <filename_with.c>
*/
#ifdef DEBUGON
#ifdef MAINDEBUG
#ifndef DEBUG
#define DEBUG
#endif
int yydebug = 0;
#endif
#endif
#include "debuglib.h"
#include <stdio.h>
#include <stdlib.h>
#include "datacg.h"
#define YYSTYPE data
#include "cg.tab.h"
#include "symtabcg.h"
#include "Listcg.h"
#include "commandcg.h"
#include "main.h"
#include "transcg.h"
#include "cg.l.h"
#include "memlib.h"
#include <string.h>
extern int yyparse(void);
char *MAIN_STRING="main";
//extern int yytables_fload(FILE *);
//extern int yytables_destroy();
//extern int yylex_destroy();
//extern int yy_delete_buffer();
int main(int argc, char **argv){
	founderror=FALSE;
	othercounter=1;
	globalcount=0;
	param_offset=0;
	infile = NULL;
	filename = NULL;
	filename = "main.c";
	initializelabel();
	if(checkargs(argc,argv) == -1){
		#ifdef DEBUG
			//filename = "input_filename.c";
			dbprint(MAINC,"Improper or no arguments given to compiler: Example filename -- ", STR, filename);
		#endif
		return -1;
	}
	filename = argv[1];
	init_mem_man();
	if((filename = openfile(argc, argv)) == NULL){
		return -1;
	}
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

	yyparse();
}while(!feof(yyin));
		#ifdef DEBUG
		dbprint(MAINC,"Finished Parsing ",STR,filename);
		#endif
		if(infile !=NULL)
			fclose(infile);
		infile=NULL;
/*	yytables_destroy();
	yylex_destroy();*/
//	free(filename);
	filename=NULL;
	mem_cleanup();
	return 0;

}
