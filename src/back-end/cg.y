/*
Code Generator Bison Parser File
*/
%{
#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "debuglib.h"
#include "typecg.h"
#include "Listcg.h"
//#include "symtabcg.h"
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
//extern Symtab* mysymtab;
int mainlabel;
Funcbcg* currentFunc;
extern int yyerror(const char *);
extern int yylex (void);
extern int warning(char*,char*);
extern int error(char*,char*);
%}
%expect 2

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
%type <value.cargvalue> command_args
%type <value.transunitvalue> func mainfunc
%type <trans_list> tunits translation_unit

%start starter
%%

starter:{
	#ifdef DEBUG
	debugprint(1,"EMPTY STARTER","");
	#endif
} 
	| tunits {
			if(founderror == FALSE){
				#ifdef DEBUG
				debugprint(1,"All Translation Units Recognized and compacted into One","");
				#endif

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
tunits: translation_unit mainfunc {
	if($1 != NULL && $2 != NULL){
		$$ = appendTransList($1,$2->name, $2->commandlist);
		#ifdef DEBUG
		debugprint(1,"Main Function Translation Unit Recognized and added to Start of Translation Units","");
		#endif
	}
}
	| mainfunc {
		$$ = mkTransList($1->name, $1->commandlist);
		#ifdef DEBUG
		debugprint(1,"Main Function Translation Unit Recognized and added to Start of Translation Units","");
		#endif
	
	}
translation_unit: func {
	if($1 != NULL){
			$$ = mkTransList($1->name, $1->commandlist);
			#ifdef DEBUG
			debugprint(1,"Function Translation Unit Recognized and added to Start of Translation Units","");
			#endif
	}
			
		}
			
	| translation_unit func {
		$$ = appendTransList($1,$2->name, $2->commandlist);
		#ifdef DEBUG
		debugprint(1,"Function Translation Unit Recognized and added to existing Translation Units","");
		#endif
	}
;

mainfunc: maint commandlist {
	if($2 != NULL){
		#ifdef DEBUG
		debugprintc(1,"Proccessing the main function at the bottom of the file with the List of Commands",$2);
		#endif
		commandlisttype *temp;
		temp = $2->list;
		if( $2 != NULL){
			$$->name = "main";
			$$->commandlist = $2;
			$$->next_trans_unit = NULL;
//			for(int i = 0; i < $<command_list>2->listsize && temp!=NULL; i++){
//				#ifdef DEBUG
//				debugprintd(1,"Generating Command ",i);
//				#endif
//				gen_instr(temp->name);
//				temp = temp->nextcommand;
//			}
		}
	}
	}
;

func: Ident commandlist {
		gen_label(genlabelw($1, getlabel() ));
		commandlisttype *temp;
		temp = NULL;
		if($2 != NULL){
			temp = $2->list;
			#ifdef DEBUG
			debugprintd(1,"Counting and Generating Instructions: ", temp->length);
			#endif
		}
		if($1 != NULL && $2 != NULL){
			$$->name = $1;
			$$->commandlist = $2;
			$$->next_trans_unit = NULL;
			for(int i = 0; i < $<command_list>2->listsize && temp!=NULL; i++){
				#ifdef DEBUG
				debugprintd(1,"Generating Command ",i);
				#endif
				gen_instr(temp->name);
				temp = temp->nextcommand;
			}
		}
	}
;
commandlist: command_name {
		#ifdef DEBUG
		debugprint(1,"A List of Commands is being added to with the first and no arguments",$1);
		#endif
		$$ = mkcommandList((char*)$1, (ListC*)NULL);
		}
	| commandlist command_name {
		#ifdef DEBUG
		debugprinta2(1,"A List of Commands is being added to with another command and no arguments",$2, NULL);
		#endif
		$$ = appendcommandList((commandList*)$1, (char*)$2, (ListC*)NULL);
		}
	| command_name command_args  {
		#ifdef DEBUG
		debugprinta2(1,"A List of Commands is being added to with the first and its arguments as a whole",$1, $2);
		#endif
		$$ = mkcommandList((char*)$1, (ListC*)$2);
		}
	| commandlist command_name command_args {
		#ifdef DEBUG
		debugprinta2(1,"A List of Commands is being added to with another command and its arguments",$2, $3);
		#endif
		$$ = appendcommandList((commandList*)$1, (char*)$2, (ListC*)$3);
		}
;

command_name: CommandName {
			#ifdef DEBUG
			debugprint(1,"CommandName found",$1);
			#endif
			$$ = (char*) $1;
	}
;


command_args: Ident {
			char *vals[2];
			vals[0] = (char*)$1;
			vals[1] = NULL;
			$$ = mklistC(vals);
			#ifdef DEBUG
			debugprint(1,"Ident inside command_args",vals[0]);
			#endif
		}
	| StrConstant {
			char *vals[2];
			vals[0] = (char*)$1;
			vals[1] = NULL;
			$$ = mklistC(vals);
			#ifdef DEBUG
			debugprint(1,"StrConstant inside command_args","");
			#endif
		}
	| IntConstant {
			int vals[2];
			vals[0] = (int)$1;
			vals[1] = (int)0;
			$$ = mklistCi(vals);
			#ifdef DEBUG
			debugprintd(1,"IntConstant inside command_args",$1);
			#endif
		}
	| command_args comma Ident {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				#ifdef DEBUG
				debugprint(1,"Ident inside command_args list version",$3);
				#endif
				$$ = appendListC($1, (char*)$3, STR);
			}
			else{
				warning("Too many arguments given to original instruction","");
			}
		}
	| command_args comma StrConstant {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				#ifdef DEBUG
				debugprint(1,"StrConst inside command_args list version",$3);
				#endif
				$$ = appendListC($1, (char*)$3, STR);
			}
			else{
				warning("Too many arguments given to original instruction","");
			}
		}
	| command_args comma IntConstant {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				#ifdef DEBUG
				debugprintd(1,"IntConstant inside command_args list version",$3);
				#endif
				$$ = appendListCi($1, (int)$3, INT);
			}
			else{
				warning("Too many arguments given to original instruction","");
			}

		}
;

%%

int yyerror(const char *s)
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

