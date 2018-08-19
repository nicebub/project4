/*scott lorberbaum
   compilers spring 2004
	This file is the specification file for bison for the uC language.  This is only the minimal amount need to create the ucc.tab.h file which specifies the tokens
and their values.  Then that file is included in the flex spec file to have identical tokens.  Also there is a declartion of a union, the Line_Number which helps keep
track of the line number, and some of the operators have their precedence specifed.
*/
%{
#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "typecg.h"
#include "Listcg.h"
#include "symtabcg.h"
#include "datacg.h"
#include "transcg.h"
#include <stdio.h>
//#define yyerrok
#define YYDEBUG 1
#define YYERROR_VERBOSE 1
#define YYSTYPE data
#include "cg.l.h"
#include "main.h"
#include <string.h>
extern int Line_Number;
boolcg founderror=FALSE;
extern int globalcount;
extern int mainlocal;
extern int othercounter;
extern int param_offset;
extern int offset_counter;
extern Symtab* mysymtab;
int mainlabel;
Funcbcg* currentFunc;
extern int yyerror(char *);
extern int yylex (void);
extern int warning(char*,char*);
extern int error(char*,char*);
%}
//%expect 1

%token Ident
%token IntConstant
%token FloatConstant
%token StrConstant
%token CommandName
%token chart
%token intt
%token maint
%token equequ
%token neq
%token leq
%token geq
%token adof
%token elip
%token lpar
%token rpar
%token lcbra
%token rcbra
%token semi
%token comma
%token lesst
%token greatt
%token equal
%token plus
%token minus
%token divide
%token star
%token uminus
//%token uplus

%right equequ neq
%right lesst greatt
%left leq geq
%left plus minus
%left divide star
%right adof
%right uminus


%type <value.svalue> Ident
%type <value.ivalue> IntConstant
%type <value.fvalue> FloatConstant
%type <value.svalue> StrConstant command_name CommandName
%type <value.ivalue> intt
%type <value.ivalue> chart adof elip lpar rpar lcbra rcbra semi comma equal
%type <value.relopvalue> lesst greatt leq geq
%type <value.addopvalue> plus minus
%type <value.eqopvalue> equequ neq
%type <value.multopvalue> divide star
%type <value.ivalue> uminus
%type <command_list> commandlist
%type <value.cargvalue> command_args optional_command_args


%start starter
%%

starter: translation_unit {
			if(founderror == FALSE){
				/*gen_label("main");
				gen_instr_I("enter",0);
				gen_instr_I("alloc",globalcount);
				gen_instr_I("enter",0);
				gen_call(genlabelw("main",mainlabel),0);
				gen_instr("return");
				*/
			}
	}
;

translation_unit: /* empty because then just a main function defined or empty file? */
	//| variabledecl {}
	| translation_unit func {}
	| translation_unit mainfunc {}
	//| translation_unit variabledecl {}
//	| error { error("unexpected token between translation units or at end of program","");}
	| translation_unit error { error("(unexpected token between translation units or at end of program)","");}
;
mainfunc: maint commandlist {
/*		gen_label(genlabelw((char*)$1.value.svalue, getlabel() ));
		commandlisttype *temp;
		temp = $2->list;
		for(int i = 0; i < temp->length; i++){
			gen_instr(temp->name);
			temp = temp->nextcommand;
		}*/
	}
;

func: Ident commandlist {
		gen_label(genlabelw($1, getlabel() ));
		commandlisttype *temp;
		temp = $2->list;
		for(int i = 0; i < temp->length; i++){
			gen_instr(temp->name);
			temp = temp->nextcommand;
		}
	}
;
commandlist: command_name optional_command_args  {
		$$ = mkcommandList((char*)$1, (ListC*)$2);
		}
	| commandlist command_name optional_command_args {
		$$ = appendcommandList((commandList*)$1, (char*)$2, (ListC*)$3);
		}
;

command_name: CommandName {
		$$ = (char*) $1;
	}
;

optional_command_args:  /* empty args */ {			
			#ifdef DEBUG
			debugprint("EMPTY inside optional_command_args","");
			#endif
			$$ = NULL;
			}
	|	command_args {
			$$ = $1; 
			#ifdef DEBUG
			debugprint("command_args matched inside optional_command_args","");
			#endif
		}
;

command_args: Ident {
			char *vals[2];
			vals[0] = (char*)$1;
			vals[1] = NULL;
			$$ = mklistC(vals);
			#ifdef DEBUG
			debugprint("Ident inside command_args","");
			#endif
		}
	| StrConstant {
			char *vals[2];
			vals[0] = (char*)$1;
			vals[1] = NULL;
			$$ = mklistC(vals);
			#ifdef DEBUG
			debugprint("StrConstant inside command_args","");
			#endif
		}
	| IntConstant {
			int vals[2];
			vals[0] = (int)$1;
			vals[1] = (int)0;
			$$ = mklistCi(vals);
			#ifdef DEBUG
			debugprint("IntConstant inside command_args","");
			#endif
		}
	| command_args comma Ident {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				$$ = appendListC($1, (char*)$3, STR);
			}
			else{
				warning("Too many arguments given to original instruction","");
			}
		}
	| command_args comma StrConstant {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				$$ = appendListC($1, (char*)$3, STR);
			}
			else{
				warning("Too many arguments given to original instruction","");
			}
		}
	| command_args comma IntConstant {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				$$ = appendListCi($1, (int)$3, INT);
			}
			else{
				warning("Too many arguments given to original instruction","");
			}

		}
;

%%

int yyerror(char *s)
{
	fprintf(stderr,"%s:%d:-> Error: %s\n", filename,Line_Number, s);
    return 0;
}


int warning(char *s1, char* s2)
{
	fprintf(stderr,"%s:%d:-> Warning: %s %s\n",filename,Line_Number,s1,s2);
    return 0;
}

int error(char* s1, char* s2){
	fprintf(stderr,"%s:%d:-> Error: %s %s\n",filename,Line_Number,s1,s2);
	founderror=TRUE;
    return 0;
}

