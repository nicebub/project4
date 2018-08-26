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
extern boolcg founderror;
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
extern int error(int which, char*,char*);
%}
//%expect 2

%token Ident
%token IntConstant
%token FloatConstant
%token StrConstant
%token CommandName
%token returnft
%token labelt
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
%token newlinet
%token eoft

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
%type <value.transunitvalue> mainfunc
%type <trans_list> tunits translation_units func
//%type <trans_list> optional_extra_w_labels with_labels
%type <value.svalue> returnft labelt

%start starter
%%

starter: zlines_eof { 
	#ifdef DEBUG
	dbprint(PARSER,"EMPTY STARTER",STR,"");
	#endif
} 
	| zlines {
		gen_filename_comment();
		gen_section_text();
		gen_printf_dec();
		gen_global_main();
//		gen_prolog_macro();
//		gen_epilog_macro();
//		gen_set_stack();
		
		
//		gen_label("_bss");
	} tunits  {
			if(founderror == FALSE){
				#ifdef DEBUG
				dbprint(PARSER,"All Translation Units Recognized and compacted into One",STR,"");
				dbprint(PARSER,"Printing all commands",STR,"");
				dbprint(PARSER,"",STR,"");
//				dbprint(PARSER, "", STR, $3);
				debugprintt(1,$3);
				#endif

				/*gen_label("main");
				gen_instr_I("enter",0);
				gen_instr_I("alloc",globalcount);
				gen_instr_I("enter",0);
				gen_call(genlabelu("main",mainlabel),0);
				gen_instr("return");
				*/
			}
	}
;
tunits: translation_units mainfunc {
	if($1 != NULL && $2 != NULL){
		$$ = appendTransList($1,$2->name, $2->commandlist);
		#ifdef DEBUG
		dbprint(PARSER,"Main Function Translation Unit Recognized and added to Start of Translation Units",STR,"");
		#endif
	}
}
	| mainfunc {
		$$ = mkTransList($1->name, $1->commandlist);
		#ifdef DEBUG
		dbprint(PARSER,"Main Function Translation Unit Recognized and added to Start of Translation Units",STR,"");
		#endif
	
	}
translation_units: func {
	if($1 != NULL){
			#ifdef DEBUG
			dbprint(PARSER,"Function Translation Unit Recognized and added to Start of Translation Units",STR,"");
			#endif

			$$ = $1;
	}
			
		}
			
	| translation_units func {
		if($1 != NULL){
			if($2 != NULL ){
				$$ = concat_trans_unit_list($1,$2);
			}
		#ifdef DEBUG
		dbprint(PARSER,"Function Translation Unit Recognized and added to existing Translation Units",STR,"");
		#endif
	}
}
;

mainfunc: maint lines commandlist {
	if($3 != NULL){
		#ifdef DEBUG
		debugprintc(1,"Proccessing the main function at the bottom of the file with the List of Commands",$3);
		#endif
		commandlisttype *temp;
		temp = NULL;
		temp = $3->list;
		if( $3 != NULL){
			$$->name = $<value.svalue>1;
			$$->commandlist = $3;
			$$->next_trans_unit = NULL;
//			for(int i = 0; i < $<command_list>3->listsize && temp!=NULL; i++){
//				#ifdef DEBUG
//				dbprint(PARSER,"Generating Command ",INT, &i);
//				#endif
//				gen_instr(temp->name);
//				temp = temp->nextcommand;
//			}
		}
	}
	}
;

//func: Ident {gen_label(genlabelu($1, getlabel() )); } commandlist optional_extra_w_labels {

func: Ident lines commandlist {
//		gen_label(genlabelu($1, getlabel() )); 
		commandlisttype *temp;
		gen_label(genlabelu($1, getlabel() )); 
		temp = NULL;
		
		if( $3 != NULL){
			temp = $3->list;
			#ifdef DEBUG
			dbprint(PARSER,"Counting and Generating Instructions: ",INT, &temp->length);
			#endif
		}
		if($1 != NULL && $3 != NULL){
//			#ifdef DEBUG
//			debugprinta2(1,"A List of Commands is being added to with another command and no arguments",$3, NULL);
//			#endif
//			$3 = appendcommandList((commandList*)$3, (char*)$3, (ListC*)NULL);
			
			
			$$ = mkTransList($1, $3);

			#ifdef DEBUG
			dbprint(PARSER,"Function Translation Unit Recognized and added to Start of Translation Units",STR,"");
			#endif

//			$$ = prependTransList($4,$1, $3);

			#ifdef DEBUG
			dbprint(PARSER,"Function Translation Unit Recognized and added to existing Translation Units",STR,"");
			#endif
			
			
			
//			$$->name = (char*)strdup($1);
//			$$->name = (char*)$1;
//			$$->commandlist = $2;
//			$$->next_trans_unit = NULL;
			for(int i = 0; i < $<command_list>3->listsize && temp!=NULL; i++){
				#ifdef DEBUG
				dbprint(PARSER,"Generating Command ",INT, &i);
				#endif
				gen_instr(temp->name);
				temp = temp->nextcommand;
			}
		}
		
	}
/* //	| Ident commandlist returnft {
	| Ident commandlist {
			gen_label(genlabelu($1, getlabel() ));
			commandlisttype *temp;
			temp = NULL;
			if($2 != NULL){
				temp = $2->list;
				#ifdef DEBUG
				dbprint(PARSER,"Counting and Generating Instructions before adding returnf: ",INT, &temp->length);
				#endif
			}
		
			if($1 != NULL && $2 != NULL){

				#ifdef DEBUG
				debugprinta2(1,"A List of Commands is being added to with another command and no arguments",$2, NULL);
				#endif

				$2 = appendcommandList((commandList*)$2, (char*)$2, (ListC*)NULL);

				$$ = mkTransList($1, $2);

				#ifdef DEBUG
				dbprint(PARSER,"Function Translation Unit Recognized and added to Start of Translation Units",STR,"");
				#endif

			
	//			$$->name = (char*)strdup($1);
	//			$$->commandlist = $2;
	//			$$->next_trans_unit = NULL;
				for(int i = 0; i < $<command_list>2->listsize && temp!=NULL; i++){
					#ifdef DEBUG
					dbprint(PARSER,"Generating Command ",INT, &i);
					#endif
					gen_instr(temp->name);
					temp = temp->nextcommand;
				}
			}
			}*/
;

/*optional_extra_w_labels: with_labels returnft {
	translation_unit *temp;
	temp = NULL;
	if($1 != NULL){
		temp = $1->list;
		if(temp!=NULL){
			temp = getlastUnit($1);
			if(temp !=NULL){
				appendcommandList((commandList*)temp->commandlist, (char*) $2, (ListC*) NULL);
			}
			else{
				error(PARSER," deep inside with labels","");
				return -1;
			}
		}
		else{
			error(PARSER,"inside with labels","");
			return -1;
		}
//		$2 = appendcommandList((commandList*)$2, (char*)$3, (ListC*)NULL);
	}	
}
;

with_labels: labelt commandlist  {
		gen_label(genlabelu($1, getlabel() ));
		commandlisttype *temp;
		temp = NULL;
		if($2 != NULL){
			temp = $2->list;
			#ifdef DEBUG
			dbprint(PARSER,"Counting and Generating Instructions before adding returnf: ",INT, &temp->length);
			#endif
		}
		
		if($1 != NULL && $2 != NULL){

//			#ifdef DEBUG
//			debugprinta2(1,"A List of Commands is being added to with another command and no arguments",$3, NULL);
//			#endif

//			$2 = appendcommandList((commandList*)$2, (char*)$3, (ListC*)NULL);

			$$ = mkTransList($1, $2);

			#ifdef DEBUG
			dbprint(PARSER,"Function Translation Unit Recognized and added to Start of Translation Units",STR,"");
			#endif

			
//			$$->name = (char*)strdup($1);
//			$$->commandlist = $2;
//			$$->next_trans_unit = NULL;
			for(int i = 0; i < $<command_list>2->listsize && temp!=NULL; i++){
				#ifdef DEBUG
				dbprint(PARSER,"Generating Command ",INT, &i);
				#endif
				gen_instr(temp->name);
				temp = temp->nextcommand;
			}
		}
}
	|	optional_extra_w_labels labelt commandlist  {

}
;
*/


commandlist: command_name lines {
		#ifdef DEBUG
		dbprint(PARSER,"A List of Commands is being added to with the first and no arguments",STR,$1);
		#endif
		$$ = mkcommandList((char*)$1, (ListC*)NULL);
		}
	| commandlist command_name lines {
		#ifdef DEBUG
		debugprinta2(1,"A List of Commands is being added to with another command and no arguments",$2, NULL);
		#endif
		$$ = appendcommandList((commandList*)$1, (char*)$2, (ListC*)NULL);
		}
	| command_name command_args lines {
		#ifdef DEBUG
		debugprinta2(1,"A List of Commands is being added to with the first and its arguments as a whole",$1, $2);
		#endif
		$$ = mkcommandList((char*)$1, (ListC*)$2);
		}
	| commandlist command_name command_args lines {
		#ifdef DEBUG
		debugprinta2(1,"A List of Commands is being added to with another command and its arguments",$2, $3);
		#endif
		$$ = appendcommandList((commandList*)$1, (char*)$2, (ListC*)$3);
		}
;

command_name: CommandName  {
			#ifdef DEBUG
			dbprint(PARSER,"CommandName found",STR,$1);
			#endif
			$$ = (char*) $1;
	}
;


command_args: Ident  {
			char *vals[2];
			vals[0] = (char*)$1;
			vals[1] = NULL;
			$$ = mklistC(vals);
			#ifdef DEBUG
			dbprint(PARSER,"Ident inside command_args",STR,vals[0]);
			#endif
		}
	| StrConstant  {
			char *vals[2];
			vals[0] = (char*)$1;
			vals[1] = NULL;
			$$ = mklistC(vals);
			#ifdef DEBUG
			dbprint(PARSER,"StrConstant inside command_args",STR,"");
			#endif
		}
	| IntConstant {
			int vals[2];
			vals[0] = (int)$1;
			vals[1] = (int)0;
			$$ = mklistCi(vals);
			#ifdef DEBUG
			dbprint(PARSER,"IntConstant inside command_args",INT, &$1);
			#endif
		}
	| command_args comma Ident {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				#ifdef DEBUG
				dbprint(PARSER,"Ident inside command_args list version",STR,$3);
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
				dbprint(PARSER,"StrConst inside command_args list version",STR,$3);
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
				dbprint(PARSER,"IntConstant inside command_args list version",INT, &$3);
				#endif
				$$ = appendListCi($1, (int)$3, INT);
			}
			else{
				warning("Too many arguments given to original instruction","");
			}

		}
;

//lines_eof: lines eoft {}
//;

lines: newlinet
	| lines newlinet
;
zlines_eof: zlines
;
zlines: /* empty */
	| lines
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


