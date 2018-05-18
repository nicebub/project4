/*scott lorberbaum
 compilers spring 2004
	this is the main.c source file that calls the yylex() function in a loop, and outputs the token name and its semantic value.  The main also checks to make sure
that the file opened on the command line arguments ends in a '.c'.  Which also basically means that to send a file it just give the name after the ucc command:
		./ucc <filename_with.c> or
		./ucc < <filename_with.c>
*/
#ifdef DEBUGON
#define DEBUG
int yydebug = 1;
#endif
#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#define YYSTYPE data
#include "ucc.tab.h"
#include "symtab.h"
#include "List.h"
#include "command.h"
#include "main.h"
#include "trans.h"
#include "ucc.l.h"
#include <string.h>
extern int yyparse(void);

int main(int argc, char **argv){
	founderror=FALSE;
	othercounter=1;
	globalcount=0;
	param_offset=0;
	initializelabel();
	if(checkargs(argc,argv) == -1){
		return -1;
	}
	if(openfile(argc, argv) == -1){
		return -1;
	}
	mysymtab = createTree(100);
	if(mysymtab == NULL){
		fprintf(stderr, "Unable to construct symbol table\n");
		return -1;
	}

	offset_counter=5;
	yyparse();

	#ifdef DEBUG
	printTree(mysymtab);
	#endif
	deleteTree(mysymtab);
	if(infile !=NULL)
		fclose(infile);
	return 0;

}
