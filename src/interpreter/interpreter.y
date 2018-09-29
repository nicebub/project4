/*
Code Generator Bison Parser File
*/
%{
    
//    #define PARSERDEBUG
#if defined(DEBUGON) && !defined(PARSERDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if defined(DEBUGON) && defined(PARSERDEBUG) && !defined(DEBUG)
#define DEBUG
#define YYDEBUG 1
//extern int yydebug = 1;
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
//#define YYDEBUG 1
//extern int yydebug = 1;
#endif


#include "interpreter.y.h"
#include "debuglib.h"
#include "typeint.h"
#include "Listint.h"
#include "dataint.h"
//#include "transint.h"
#include "memlib.h"

//#include "symtabint.h"

#include <stdio.h>
#include <string.h>

#define ARGS_CMD_PARSE(intype,newlist,invalue,if_nt ,isfloat,...) {\
intype vals[2];\
vals[0] = (intype)invalue;\
vals[1] = (intype)NULL;\
newlist = mklistC##if_nt(vals,isfloat);}



int mainlabel;
Funcbcg* currentFunc;
trans_u_list *All_T_Units;
translation_unit *trans_array[MAX_FUNCTIONS];
int translation_unit_amount;
%}

%token Ident
%token IntConstant
%token FloatConstant
%token StrConstant
%token CommandName
%token maint
%token comma
%token uminus
%token newlinet

%right uminus


%type <value.svalue> Ident
%type <value.ivalue> IntConstant
%type <value.fvalue> FloatConstant
%type <value.svalue> StrConstant command_name CommandName

%type <value.ivalue> comma

%type <value.ivalue> uminus
%type <command_list> commandlist
%type <value.cargvalue> command_args
%type <trans_list> tunits translation_units func starter mainfunc


%start starter
%%

starter: zlines_eof { 
	$$ = NULL;

	All_T_Units = NULL;

	#ifdef DEBUG
	dbprint(PARSER,"EMPTY STARTER",0);
	#endif
} 
	| zlines {
		if(founderror == FALSE){
		//	gen_section_text();
		}		
	  } 
		
	  tunits  {
		  $$ = $3;
		  All_T_Units = (trans_u_list*) $3;
		  
			if(founderror == FALSE){
				#ifdef DEBUG
				dbprint(PARSER,"All Translation Units Recognized and compacted into One",0);
				dbprint(PARSER,"Printing all commands",0);
				dbprint(PARSER,"-----------------------",0);
				#endif

			//	gen_end_prog();
								
			}
	}
;
tunits: translation_units mainfunc {
	if($1 != NULL && $2 != NULL){
		ADD_TO_TRANS_ARRAY($2)

		$$ = concat_trans_unit_list($1,$2);
		
		#ifdef DEBUG
		dbprint(PARSER,"Main Function Translation Unit Recognized and added to Start of Translation Units",0);
		#endif
	}
}
	| mainfunc {
		if($1 != NULL){

			ADD_TO_TRANS_ARRAY($1)

			$$ = $1;
			#ifdef DEBUG
			dbprint(PARSER,"Main Function Translation Unit Recognized and added to Start of Translation Units",0);
			#endif
		}
	}
	
translation_units: func {
	if($1 != NULL){
			#ifdef DEBUG
			dbprint(PARSER,"Function Translation Unit Recognized and added to Start of Translation Units",0);
			#endif

			ADD_TO_TRANS_ARRAY($1)

			$$ = $1;
	}
			
		}
			
	| translation_units func {
		if($1 != NULL){
			if($2 != NULL ){

				ADD_TO_TRANS_ARRAY($2)

				$$ = concat_trans_unit_list($1,$2);
			}
		#ifdef DEBUG
		dbprint(PARSER,"Function Translation Unit Recognized and added to existing Translation Units",0);
		#endif
	}
}
;

mainfunc: maint lines commandlist {
    commandlisttype *temp;
    translation_unit *temp_unit;
    char * pointer;
    temp = NULL;
    pointer = NULL;
    temp_unit = NULL;
	if($3 != NULL){
		#ifdef DEBUG
		debugprintc(1,"Proccessing the main function at the bottom of the file with the List of Commands",$3);
		#endif
		
		temp = $3->list;
		
		if( $3 != NULL){
		    
		    pointer = $<value.svalue>1;
		    REQUESTMEM(temp_unit, translation_unit, GENERIC, *sizeof(translation_unit))
		    REQUESTMEM(temp_unit->name, char, STR, *strlen(pointer)+1)
		    strlcpy(temp_unit->name,pointer, strlen(temp_unit->name)+1);
			temp_unit->next_trans_unit = NULL;
			$$= mkTransList($<value.svalue>1,$3);
		}
	}
	}
;


func: Ident lines commandlist {
		commandlisttype *temp;
		if(founderror == FALSE){
		}
		temp = NULL;
		
		if( $3 != NULL){
			temp = $3->list;
			#ifdef DEBUG
			dbprint(PARSER,"Counting and Generating Instructions: ",1, INT, temp->length);
			#endif
		}
		if($1 != NULL && $3 != NULL){
			
			$$ = mkTransList($1, $3);

			#ifdef DEBUG
			dbprint(PARSER,"Function Translation Unit Recognized and added to Start of Translation Units",0);
			#endif


			#ifdef DEBUG
			dbprint(PARSER,"Function Translation Unit Recognized and added to existing Translation Units",0);
			#endif
			
			
				if(founderror == FALSE){

			//	check_and_gen_if_main((char*)$1);

			//	gen_label(genlabelu($1, getlabel() ));

			//	gen_func_prolog();

			//	gen_set_stack(); 

			//	gen_run_commands($3);

			//	gen_reverse_stack();
				
			//	gen_func_epilog();
			}
		}
		
	}
;


commandlist: command_name lines {
		#ifdef DEBUG
		dbprint(PARSER,"A List of Commands is being added to with the first and no arguments",1, STR,$1);
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
    char *pointer;
    pointer = NULL;
			#ifdef DEBUG
			dbprint(PARSER,"CommandName found",1, STR,$1);
			#endif
			REQUESTMEM(pointer, char, STR, *strlen($1)+1)
			strlcpy(pointer,$1, strlen(pointer)+1);
//			$$ = (char*) $1;
	}
;



command_args: Ident  {
    ARGS_CMD_PARSE(char*, $$,$1, ,1, STR,vals[0])
    #ifdef DEBUG
  //  dbprint(PARSER,"Ident inside command_args",1, STR,vals[0]);
    #endif
}
	| StrConstant{

	    ARGS_CMD_PARSE(char*, $$,$1, ,0)
	    #ifdef DEBUG
	    dbprint(PARSER,"StrConstant inside command_args",0);
	    #endif
		}
	| IntConstant {

		ARGS_CMD_PARSE(int, $$,$1,i ,1, INT, $1)
		#ifdef DEBUG
		dbprint(PARSER,"IntConstant inside command_args",1, INT, $1);
		#endif
		}
	| command_args comma Ident {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				#ifdef DEBUG
				dbprint(PARSER,"Ident inside command_args list version",1, STR,$3);
				#endif
				$$ = appendListC($1, (char*)$3, STR);
			}
			else{
				warning(PARSER, "Too many arguments given to original instruction","");
			}
		}
	| command_args comma StrConstant {
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				#ifdef DEBUG
				dbprint(PARSER,"StrConst inside command_args list version",1, STR,$3);
				#endif
				$$ = appendListC($1, (char*)$3, STR);
			}
			else{
				warning(PARSER, "Too many arguments given to original instruction","");
			}
		}
	| command_args comma IntConstant {
	    #ifdef DEBUG
	    dbprint(PARSER,"found another int constant argument", 1, INT, $<value.ivalue>3);
	    dbprint(PARSER, "", 0);
	    #endif
			if($1!=NULL && $1->list !=NULL && $1->list->val[1]==NULL){
				#ifdef DEBUG
				dbprint(PARSER,"IntConstant inside command_args list version",1, INT, $<value.ivalue>3);
				#endif
				$$ = appendListCi($1, (int)$3, INT);
			}
			else{
				warning(PARSER, "Too many arguments given to original instruction","");
			}

		}
;

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

extern void init_trans_array(void){
	for(int i = 0; i < MAX_FUNCTIONS; i++) trans_array[i] = NULL; 
	translation_unit_amount =0;
}
