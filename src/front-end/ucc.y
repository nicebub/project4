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
#include "type.h"
#include "List.h"
#include "symtab.h"
#include "data.h"
#include "trans.h"
#include <stdio.h>
//#define yyerrok
#define YYDEBUG 0
#define YYERROR_VERBOSE 1
#define YYSTYPE data
#include "ucc.l.h"
#include "main.h"
#include <string.h>
extern int Line_Number;
bool founderror=FALSE;
extern int globalcount;
extern int mainlocal;
extern int othercounter;
extern int param_offset;
extern int offset_counter;
extern Symtab* mysymtab;
int mainlabel;
Funcb* currentFunc;
extern int yyerror(char *);
extern int yylex (void);
extern int warning(char*,char*);
extern int error(char*,char*);
%}
%expect 1

%token Ident
%token IntConstant
%token FloatConstant
%token StrConstant
%token voidt
%token intt
%token floatt
%token chart
%token ift
%token whilet
%token returnt
%token elset
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


%type <value.lstvalue> identlist
%type <value.relopvalue> relop
%type <value.multopvalue> mulop
%type <value.addopvalue> addop
%type <value.eqopvalue> eqop
%type <value.svalue> Ident
%type <value.ivalue> IntConstant
%type <value.fvalue> FloatConstant
%type <value.svalue> StrConstant
%type <value.ivalue> whilet ift elset
%type <value.ivalue> intt
%type <value.fvalue> floatt
%type <value.ivalue> chart voidt adof elip lpar rpar lcbra rcbra semi comma equal
%type <value.relopvalue> lesst greatt leq geq
%type <value.addopvalue> plus minus
%type <value.eqopvalue> equequ neq
%type <value.multopvalue> divide star
%type <value.ivalue> uminus
%type <value.lstpvalue> paramdeflist
%type <value.funcheadervalue> funcheader
//%type <value.lstevalue> exprlist
%type <value.exprvalue> expr
//%type <value.exprvalue>simpleexpr relexpr equalexpr
//%type <data> equalexpr relexpr simpleexpr
//%type <data> stmt ifexprstmt
//%type <data> term factor function_call func_call_with_params name_and_params constant


%start starter
%%

starter: translation_unit {
			if(founderror == FALSE){
				gen_label("main");
				gen_instr_I("enter",0);
				gen_instr_I("alloc",globalcount);
				gen_instr_I("enter",0);
				gen_call(genlabelw("main",mainlabel),0);
				gen_instr("return");

			}
	}
;

translation_unit: func {}
	| variabledecl {}
	| translation_unit func {}
	| translation_unit variabledecl {}
//	| error { error("unexpected token between translation units or at end of program","");}
	| translation_unit error { error("(unexpected token between translation units or at end of program)","");}
;

func: funcheader {
					Entry* tempEntry;
					Funcb* tempb;
					//Funcb* found;
					int a;
					ListP* templist;
                    templist= NULL;
					//type tempparam;
					listnodeP* tempnode;
					#ifdef DEBUG
					printTree(mysymtab);
					#endif
					bool stop=FALSE;
					bool alreadyopen=FALSE;
                    tempnode = NULL;
					if(strcmp("main", (char*)($1->name)) ==0){
			                        #ifdef DEBUG
                			        fprintf(stderr,"hello from inside\n");
             				        #endif
						templist = (ListP*)$1->paramlist;
	//main function
						if($1->returntype != INT)
							error("Main function has to have int as return type","");
						if(templist != NULL && templist->listsize != 1)
							error("Main function only has one parameter","");
						else if($1->ttype != VOID)
							error("Main function has to have one parameter of void","");
						#ifdef DEBUG							
						fprintf(stderr, "opening up a new scope\n");
						#endif		
						openscope(mysymtab);
						 alreadyopen=TRUE;
						if(founderror==FALSE){
							mainlabel=getlabel();
							gen_label(genlabelw("main", mainlabel));
						}
					}
					else{
						if((tempb=(Funcb*)lookup((char*)($1->name), mysymtab)) ==NULL)
							error("Function name not in symbol table","");
						else{
							Entry *tempEn; Entry * tempEn2;
							tempEn2 = (Entry*) malloc(sizeof(Entry));
							tempEn2->name = $1->name;
							tempEn = lookupB((char*)($1->name),mysymtab);
							//tempEn = *(Entry**)tfind((void*)tempEn2, (void**) &(mysymtab->Stack[mysymtab->actualStacksize -1]),Ecmp);
							if(tempEn!=NULL && tempEn->self == FUNC){
								if(tempb->returntype != $<value.funcheadervalue>1->returntype)
									error("Function declared with different return type","");
								else{
									templist= (ListP*)$<value.funcheadervalue>1->paramlist;
									if(tempb->num_param == -1)
										error("Function cannot have those parameters","");
									else if( templist!=NULL && (templist->listsize) != tempb->num_param)
										error("Function has different number of parameters","");
									else{
										if(templist!=NULL){
											tempnode = (listnodeP*)templist->list;
											for(a=0;a<templist->listsize && a<tempb->num_param &&  stop!=TRUE;a++){
												if(tempb->param_type[a] != tempnode->ttype){
													fprintf(stderr,"Error: Line: %d: argument %d: has different parameter type than in function declaration\n",Line_Number,(a+1));
													fprintf(stderr, "\nThey are %d and %d\n", tempb->param_type[a], tempnode->ttype);
													stop=TRUE;
												}
												tempnode = (listnodeP*)tempnode->nextnode;
											}
										}
										if(stop!= TRUE){
											if(tempb->bodydef == TRUE){
												error("Function definition is previously declared","");
											}
											else{
												//tempb->localcount=offset_counter;
												openscope(mysymtab);
												alreadyopen=TRUE;
												if(templist!=NULL){
													tempnode= (listnodeP*)templist->list;
												#ifdef DEBUG
												if(tempnode->ttype != VOID){
												    fprintf(stderr,"in funcheader before funcbody, param val and type is ");
				//								    fprintf(stderr,"%s %s\n",tempnode->val,(char*)tempnode->ttype);
												}
												else{
												    fprintf(stderr,"in funcheader before funcbody type is ");
												    fprintf(stderr,"%s\n",(char*)tempnode->ttype);
												}
												#endif
												}
												if(templist!=NULL){
													for(a=0;a<templist->listsize;a++){
														tempEntry = createParam((char*)strdup((char*)tempnode->val), tempnode->ttype,(offset_counter));
														install(tempEntry, mysymtab);
														offset_counter++;
														tempnode = (listnodeP*)tempnode->nextnode;
													}
												}
												if(founderror==FALSE){
													//tempb->label=getlabel();
													gen_label(genlabelw($1->name,tempb->label ));

												}
											}
										}
										else error("Stopped","");

									}
								}
							}
							else
								error("Not a function", "");
							if(tempEn2!=NULL){ free(tempEn2); tempEn2=NULL;}
						currentFunc=tempb; //fprintf(stderr,"createFunc: return type %d\ntempb: return type %d\n",currentFunc->returntype, tempb->returntype);
						}
						if(alreadyopen==FALSE) openscope(mysymtab);
					}
		}
	funcbody {
			if(founderror==FALSE){
				gen_instr("returnf");
			}
//			if(currentFunc == NULL)
//				closemainscope(mysymtab);
//			else
				closescope(mysymtab);
	}
	| funcheader semi{
			   Entry * temp; Funcb* found; int a;
			   #ifdef DEBUG
				printTree(mysymtab);
			   #endif
			   temp =  createFunc((char*)strdup($1->name), $1->returntype, (ListP*)($1->paramlist));
			   if($1->paramlist !=NULL) deleteListP((ListP*)($1->paramlist));
			   if($1 !=NULL) free($1);
			   if( strcmp("main", (char*)temp->name)==0){
					if( ((Funcb*)(temp->binding))->returntype != INT)
						error("Main function needs return type of int","");
					if( ((Funcb*)(temp->binding))->num_param != 1)
						error("Main function only takes 1 parameter","");
					if( ((Funcb*)(temp->binding))->param_type[0] != VOID)
						error("Main function parameter has to be void","");
					deleteEntry(temp);
			   }
			   else{
				   found = lookup((char*)temp->name, mysymtab);
                   #ifdef DEBUG
					if(temp->name ==NULL)
                  	 fprintf(stderr,"FUNCTION: temp before symbol table: %s\n", "NULL");
					 else
                  	 fprintf(stderr,"FUNCTION: temp before symbol table: %s\n", temp->name);
					 #endif
                   Funcb * paramOftemp = temp->binding;
                   #ifdef DEBUG
                   fprintf(stderr,"binding of temp as Funcb: value num_param in symbol table: %d\n", (paramOftemp->num_param));
                   fprintf(stderr,"param_type of paramOftemp as typeOftemp: value param_type in symbol table: %d\n", (paramOftemp->param_type[2]));
                   #endif
				   if(found == NULL){ install(temp, mysymtab); /*printTree(mysymtab)*/;}
				   else{
					if(((Funcb*)(temp->binding))->returntype != found->returntype)
						error("Function already declared with different return type","");
					if(((Funcb*)(temp->binding))->num_param == -1 || found->num_param == -1){
						if(((Funcb*)(temp->binding))->param_type[0] != found->param_type[0])
							error("In Function %s ", $1->name);
							fprintf(stderr,"argument 0 is of different type than in previous declaration\n");
					}
					else{
						if(((Funcb*)(temp->binding))->num_param != found->num_param)
							error("Function already decleared with different number of parameters","");
						if( ((Funcb*)(temp->binding))->num_param > 0 && found->num_param >0){
							for(a=0;a<((Funcb*)(temp->binding))->num_param && a<found->num_param ;a++){
								if(((Funcb*)(temp->binding))->param_type[a] != found->param_type[a]){
									error("In Function %s ", $1->name);
									fprintf(stderr, "argument %d is of different type than in previous declaration\n", a);
								}

							}
						}
					}
				   deleteEntry(temp);
				   }
			   }
			}
	| funcheader error semi { yyerrok;
					error("(unexpected token before semi in function)","");
					if($1 !=NULL) if($1->paramlist !=NULL) deleteListP((ListP*)($1->paramlist));
					if($1 !=NULL) free($1); $1 = NULL;
				}
;


funcheader: voidt Ident lpar paramdef rpar {	$$ = (funcheadertype*)malloc(sizeof(funcheadertype));
						$$->returntype = VOID;
						$$->name = $2;
                        $$->paramlist=NULL;
                        if($4.ttype == VOID) $$->ttype = VOID;
						//(ListP*)($$->paramlist) = $<value.lstpvalue>4;
						ListP * tempLP = NULL;
						tempLP = $<value.lstpvalue>4;
                        $$->paramlist = tempLP;
						}
	| intt Ident lpar paramdef rpar {	$$ = (funcheadertype*)malloc(sizeof(funcheadertype));
						$$->returntype = INT;
						$$->name = $2;
                        $$->paramlist = NULL;
						if($4.ttype == VOID) $$->ttype = VOID;
						//(ListP*)($$->paramlist) = $<value.lstpvalue>4;
						ListP * tempLP = NULL;
						tempLP = $<value.lstpvalue>4;
                        $$->paramlist = tempLP;
						}
	| floatt Ident lpar paramdef rpar {	$$ = (funcheadertype*)malloc(sizeof(funcheadertype));
						$$->returntype = FLOAT;
						$$->name = $2;
                        if($4.ttype == VOID) $$->ttype = VOID;
						//(ListP*)($$->paramlist) = $<value.lstpvalue>4;
						ListP * tempLP = NULL;
						tempLP = $<value.lstpvalue>4;
                        $$->paramlist = tempLP;
						}
	| voidt error rpar { ListP* tempP; yyerrok;
				$$ =(funcheadertype*) malloc(sizeof(funcheadertype));
				$$->name ="";
				$$->returntype=VOID;
				//(ListP*)($$->paramlist) = (ListP*) malloc(sizeof(ListP));
				ListP * tempLP;
				tempLP = (ListP*) mklistP((char*)strdup("error"), VOID);
				tempP= (ListP*)tempLP;
				$$->paramlist= tempLP;
				error("(expecting lpar before rpar in function)","");
			}
	| intt error rpar { ListP* tempP; yyerrok;
				$$ =(funcheadertype*) malloc(sizeof(funcheadertype));
				$$->name ="";
				$$->returntype=INT;
				//(ListP*)($$->paramlist) = (ListP*) malloc(sizeof(ListP));
				ListP * tempLP;
				tempLP = (ListP*) mklistP((char*)strdup("error"), VOID);
				tempP= (ListP*)tempLP;
				$$->paramlist= tempLP;
				error("(expecting lpar before rpar in function)","");
			}
	| floatt error rpar { ListP* tempP; yyerrok;
				$$ =(funcheadertype*) malloc(sizeof(funcheadertype));
				$$->name ="";
				$$->returntype=FLOAT;
				//(ListP*)($$->paramlist) = (ListP*) malloc(sizeof(ListP));
				ListP * tempLP;
				tempLP = (ListP*) mklistP((char*)strdup("error"), VOID);
				tempP= (ListP*)tempLP;
				$$->paramlist= tempLP;
				error("(expecting lpar before rpar in function)","");
			}
	| voidt Ident lpar error rpar  {ListP* tempP; yyerrok;
				$$ =(funcheadertype*) malloc(sizeof(funcheadertype));
				$$->name =$2;
				$$->returntype=VOID;
				//(ListP*)($$->paramlist) = NULL;
				ListP * tempLP1 = (ListP *)$$->paramlist;
				tempLP1 = NULL;
				//(ListP*)($$->paramlist) = (ListP*) malloc(sizeof(ListP));
				ListP * tempLP;
				tempLP = (ListP*) mklistP((char*)strdup("error"), VOID);
				tempP= (ListP*)tempLP;
				$$->paramlist= tempLP;
				error("(unexpected token after lpar and before rpar in function)","");
			}
	| floatt Ident lpar error rpar { ListP* tempP; yyerrok;
				$$ =(funcheadertype*) malloc(sizeof(funcheadertype));
				$$->name =$2;
				$$->returntype=FLOAT;
				//(ListP*)($$->paramlist) = NULL;
				ListP * tempLP1 = (ListP *)$$->paramlist;
				tempLP1 = NULL;
				//(ListP*)($$->paramlist) = (ListP*) malloc(sizeof(ListP));
				ListP * tempLP;
				tempLP = (ListP*) mklistP((char*)strdup("error"), VOID);
				tempP= (ListP*)tempLP;
				$$->paramlist= tempLP;
				error("(unexpected token after lpar and before rpar in function)","");
			}
	| intt Ident lpar error rpar {ListP* tempP; yyerrok;
				$$ = (funcheadertype*)  malloc(sizeof(funcheadertype));
				$$->name=$2;
				$$->returntype=INT;
				//(ListP*)($$->paramlist) = NULL;
				ListP * tempLP1 = (ListP *)$$->paramlist;
				tempLP1 = NULL;
				//(ListP*)($$->paramlist) = (ListP*) malloc(sizeof(ListP));
				ListP * tempLP;
				tempLP = (ListP*) mklistP((char*)strdup("error"), VOID);
				tempP= (ListP*)tempLP;
				$$->paramlist= tempLP;
				error("(unexpected token after lpar and before rpar in function)","");
			}
;


paramdef: paramdeflist {$<value.lstpvalue>$= $1;
				}
	| paramdeflist comma elip {
				$<value.lstpvalue>$ = appendListP($1, "...", VOID);
                #ifdef DEBUG
                printListP($<value.lstpvalue>$);
                #endif

				}
	| voidt {$<value.lstpvalue>$ = NULL; $$.ttype = VOID;}
	| paramdeflist error rpar { yyerrok;
					error("(unexpected token before rpar in parameter definition)","");
					deleteListP($1);
						}
	| paramdeflist comma error rpar { yyerrok;
					error("(unexpected token before rpar in parameter definition)","");
					deleteListP($1);
					}
;

paramdeflist: intt Ident  {
				$$ = mklistP((char*)strdup($2), INT);
                #ifdef DEBUG
                printListP($$);
                #endif
				}
		| floatt Ident  {
				$$ = mklistP((char*)strdup($2), FLOAT);
                #ifdef DEBUG
                printListP($$);
                #endif
				}
		| chart star Ident {
				$$ = mklistP((char*)strdup($3), STR);
                #ifdef DEBUG
                printListP($$);
                #endif
                }
	| paramdeflist comma intt Ident {
				$$ = appendListP($1, (char*)strdup($4),INT);
                #ifdef DEBUG
                printListP($$);
                #endif
				}
	| paramdeflist comma floatt Ident {
				$$ = appendListP($1, (char*)strdup($4), FLOAT);
                #ifdef DEBUG
                printListP($$);
                #endif
				}
	| paramdeflist comma chart star Ident {
				$$ = appendListP($1, (char*)strdup($5), STR);
                #ifdef DEBUG
                printListP($$);
                #endif
				}
;


funcbody: lcbra
	decls {
			if(founderror==FALSE){
				mainlocal=offset_counter-5;
				if(currentFunc==NULL)
					gen_instr_I("alloc", mainlocal);
				else{
					currentFunc->localcount=offset_counter-5-currentFunc->num_param;
					gen_instr_I("alloc",currentFunc->localcount);
				}
			}
		}
	stmtlist
	rcbra {
					#ifdef DEBUG
					printTree(mysymtab);
					#endif
					}
//	| lcbra decls stmtlist error rcbra
//		{ yyerrok;
//			error("(unexpected token before rcbra in function)","");
//		}
;


decls:  /*empty*/
	| decls variabledecl { //$<value.lstvalue>$ = $<value.lstvalue>2;
				}
	| decls func {}
;


variabledecl: intt identlist semi { if($<value.lstvalue>2 !=NULL) addtosymtab(mysymtab, INT, $<value.lstvalue>2);
					#ifdef DEBUG
						fprintf(stderr,"Found a single Integer declaration or a list of integers being declared\n");
						printTree(mysymtab);
					#endif
					}
	| floatt identlist semi { if($<value.lstvalue>2 !=NULL) addtosymtab(mysymtab, FLOAT, $<value.lstvalue>2);
					#ifdef DEBUG
					printTree(mysymtab);
					#endif
				}
	| error semi { yyerrok; error("(unexpected token in variable decl)",""); }
;


stmtlist: stmtlist stmt {}
	| stmt {}
;

stmt: expr semi {
		if(founderror==FALSE)
			gen_instr_I("popI",4);
	}
	| returnt semi {
					if(currentFunc ==NULL)
						error("main function has to return a value","");
					else{
						if(currentFunc->returntype != VOID)
							error("Function has return type that is not void","");
						else{
							if(founderror==FALSE){
								gen_instr("returnf");
							}
						}
					}
			}
	| returnt expr semi {		if($2->numeric != TRUE)
						error("non numeric expression in return statement or return type is void", "");
					else{
						if(currentFunc ==NULL){
							if($2->type != INT)
								warning("main function has int return type","");

								if(founderror==FALSE){
									if($2->lval==TRUE){
										switch($2->type){
											case INT:	gen_instr("fetchI"); break;
											case FLOAT:	gen_instr("fetchR"); break;
                                            default: break;
										}
									}
									if($2->type != INT) gen_instr("int");
									gen_instr("setrvI");
								}

						}
						else{
							#ifdef DEBUG
							printf("type and returntype : %d: %d\n",$2->type,currentFunc->returntype);
							#endif
							if($2->type != currentFunc->returntype)
								warning("function has different returntype","");

								if(founderror==FALSE){
                                                if($2->lval==TRUE){
                                                    switch($2->type){
                                                        case INT:       gen_instr("fetchI"); break;
                                                        case FLOAT:     gen_instr("fetchR"); break;
                                                        default:    break;
                                                                                }
                                                                        }
									switch(currentFunc->returntype){
										case INT:	switch($2->type){
													case FLOAT:	gen_instr("int");
													case INT:	gen_instr("setrvI"); break;
                                                    default: break;
												}
												break;
										case FLOAT:	switch($2->type){
													case INT:	gen_instr("flt");
													case FLOAT:	gen_instr("setrvR"); break;
                                                    default:    break;
												}
												break;
                                        default: break;
									}
                                    
									gen_instr("returnf");
                                                                }



						}
					}

				}
	| whilet {
		if(founderror==FALSE){
			$$.one= othercounter;
			othercounter++;
			$$.two = othercounter;
			othercounter++;
			gen_label(genlabelw("",$$.one));
		}
	}
	lpar
	expr
	rpar {
		if(founderror==FALSE){
			if($4->numeric==TRUE){
				if($4->lval==TRUE ){
					switch($4->type){
						case INT:	gen_instr("fetchI"); break;
						case FLOAT:	gen_instr("fetchR"); gen_instr("int"); break;
                        default:    break;
					}
				}
			}
			gen_instr_S("jumpz", genlabelw("",$2.two));

		}
	}
	stmt {
				if($4->numeric !=TRUE)
					error("non numeric expression in while statement","");
				else{
						if($4->type !=INT)
							error("expression in while statement is not an integer","");
						if(founderror==FALSE){
							gen_instr_S("jump", genlabelw("",$2.one));
							gen_label(genlabelw("",$2.two));
						}

				}
	}

	| ifexprstmt
	  elset {
		if(founderror==FALSE){
			gen_instr_S("jump", genlabelw("",$1.two));
			gen_label(genlabelw("",$1.one));
		}
	}

	stmt {
				if($1.numeric !=TRUE)
						error("non numeric expression in if statement","");
					else{
						if($1.ttype !=INT)
							error("expression in if statement is not an integer","");
						else{
							if(founderror==FALSE){
								gen_label(genlabelw("",$1.two));
							}
						}
					}
	}

	| ifexprstmt{
					if($1.numeric !=TRUE)
						error("non numeric expression in if statement","");
					else{
						if($1.ttype !=INT)
							error("expression in if statement is not an integer","");
						else{

							if(founderror==FALSE) gen_label(genlabelw("",$1.one));
						}
					}
	}

	| lcbra stmtlist rcbra {
				//closescope(mysymtab);
				}
	| returnt error { yyerrok; error("(unexpected token after return in return stmt)","");}
	| whilet error semi { yyerrok; error("(unexpected token before semi in while stmt)","");}
	//| whilet lpar expr error stmt { yyerrok; error("(unexpected token between expr and stmt in while stmt)","");}
	| ift error stmt { yyerrok; error("(unexpected token before stmt in if stmt)","");}
	//| ift lpar expr error stmt { yyerrok; error("(unexpected token before stmt in if stmt)","");}
	| lcbra stmtlist error rcbra { yyerrok;
					error("(unexpected token before rcbra in stmt)","");
					//closescope(mysymtab);
				}
;

ifexprstmt:
	ift
	lpar
	expr
	{
		$$.one=othercounter;
		othercounter++;
		$$.two=othercounter;
		othercounter++;
		if(founderror==FALSE){
			if($3->lval==TRUE && $3->numeric==TRUE){
				switch($3->type){
					case INT:	gen_instr("fetchI"); break;
					case FLOAT:	gen_instr("fetchR"); gen_instr("int"); break;
                    default:    break;
				}
			}
			gen_instr_S("jumpz", genlabelw("",$$.one));
		}
	}
	rpar
	stmt {
		$$.lval=$3->lval; $$.numeric=$3->numeric; $$.ttype = $3->type; $$.one = $4.one; $$.two=$4.two;
	}
;

expr: equalexpr equal equalexpr {
					if($1.lval !=TRUE){
						error("Cannot make assignment. Left hand side is not a correct lval","");
					}
					else if($3.numeric !=TRUE){
						error("Cannot make assignment, Right hand side is not numeric.","");
					}
					else {
						if(founderror==FALSE){
							if($3.lval==TRUE){
								switch($3.ttype){
									case INT:	gen_instr("fetchI"); break;
									case FLOAT:	gen_instr("fetchR"); break;
                                    default:    break;
								}
							}
						}
						if(($1.ttype==INT && $3.ttype ==INT) || ($1.ttype == FLOAT && $3.ttype==FLOAT)) {
							$$ = (exprtype*) malloc(sizeof(exprtype));
							if(founderror==FALSE){
								switch($1.ttype){
									case(INT): $$->type=INT; gen_instr("storeI"); break;
									case(FLOAT): $$->type=FLOAT; gen_instr("storeR"); break;
                                    default:    break;
								}
							}
							$$->lval = TRUE;
							$$->numeric =TRUE;

						}
						else if($1.ttype ==INT && $3.ttype==FLOAT){
							warning("expressons are of different type, data may be lost","");
							$$ = (exprtype*) malloc(sizeof(exprtype));
							$$->type = INT;
							$$->lval=TRUE;
							$$->numeric=TRUE;
							if(founderror==FALSE){
								gen_instr("int");
								gen_instr("storeI");
							}
						}
						else if($1.ttype ==FLOAT && $3.ttype ==INT) {
							warning("expression are of different type, data may be lost","");
							$$ = (exprtype*) malloc(sizeof(exprtype));
							$$->type=FLOAT;
							$$->numeric=TRUE;
							$$->lval=TRUE;
							if(founderror==FALSE){
								gen_instr("flt");
								gen_instr("storeR");
							}
						}

					}
				}
	| equalexpr {
			$$ = (exprtype*) malloc(sizeof(exprtype));
			$$->lval = $1.lval; $$->numeric = $1.numeric; $$->type =$1.ttype;
			}
	| equalexpr equal error { yyerrok; error("(unexpected token after equal operator in expr)","");}
;

equalexpr: relexpr
	eqop {
		if(founderror==FALSE){
			if($1.numeric==TRUE){
				switch($1.ttype){
					case INT:	if($1.lval==TRUE) gen_instr("fetchI"); break;
					case FLOAT:	if($1.lval==TRUE) gen_instr("fetchR"); break;
                    default:    break;
				}
			}
		}
	}
	relexpr {
		if(founderror==FALSE){
			if($4.numeric==TRUE){
				switch($4.ttype){
					case INT:	if($4.lval==TRUE) gen_instr("fetchI"); break;
					case FLOAT:	if($4.lval==TRUE) gen_instr("fetchR"); break;
                    default:    break;
				}
			}
		}
					$$.lval = FALSE;
					if($1.numeric==TRUE && $4.numeric==TRUE){
						$$.numeric = TRUE;
						$$.ttype=INT;
						if(($1.ttype==INT && $4.ttype ==INT) || ($1.ttype == FLOAT && $4.ttype==FLOAT)) {$$.ttype=INT;
							if(founderror==FALSE){
                                                                        switch($<value.relopvalue>2){
                                                                                case NEQ:       if($1.ttype==INT) gen_instr("neI");
                                                                                                else if($1.ttype==FLOAT) gen_instr("neR");
                                                                                                break;
                                                                                case EQEQ:      if($1.ttype==INT) gen_instr("eqI");
                                                                                                else if($1.ttype==FLOAT) gen_instr("eqR");
                                                                                                break;
                                                                                default:    break;
                                                                        }
                                                        }

						}
						else if($1.ttype ==INT && $4.ttype==FLOAT){
							warning("expressons are of different type, data may be lost","");
							$$.ttype = INT;
								if(founderror==FALSE){
                                                                        switch($<value.relopvalue>2){
                                                                                case NEQ:       gen_instr("fltb");
                                                                                                gen_instr("neR");
                                                                                                break;
                                                                                case EQEQ:       gen_instr("fltb");
                                                                                                gen_instr("eqR");
                                                                                                break;
                                                                                default:    break;
                                                                        }
                                                                }

						}
						else if($1.ttype ==FLOAT && $4.ttype ==INT) {
							 warning("expression are of different type, data may be lost","");
							$$.ttype=INT;
							if(founderror==FALSE){
                                                                        switch($<value.relopvalue>2){
                                                                                case NEQ:       gen_instr("flt");
                                                                                                gen_instr("neR");
                                                                                                break;
                                                                                case EQEQ:       gen_instr("flt");
                                                                                                gen_instr("eqR");
                                                                                                break;
                                                                                default:    break;
                                                                        }
                                                         }
						}
					}
					else{ error("non numeric in operation",""); $$.numeric=TRUE; }
				}
	| relexpr { $$.lval = $1.lval; $$.ttype = $1.ttype; $$.numeric= $1.numeric;}
	| relexpr eqop error { yyerrok; error("(unexpected token after equality operator in expr)","");}
;

relexpr: simpleexpr
	relop {
		if(founderror==FALSE){
			if($1.numeric==TRUE){
				switch($1.ttype){
					case INT:	if($1.lval==TRUE) gen_instr("fetchI"); break;
					case FLOAT:	if($1.lval==TRUE) gen_instr("fetchR"); break;
                    default:    break;
                }
			}
		}
	}
	simpleexpr  {
			if(founderror==FALSE){
				if($4.numeric==TRUE){
					switch($4.ttype){
						case INT:	if($4.lval==TRUE) gen_instr("fetchI"); break;
						case FLOAT:	if($4.lval==TRUE) gen_instr("fetchR"); break;
                        default:    break;
					}
				}
			}
						$$.lval = FALSE;
						if($1.numeric == TRUE && $4.numeric ==TRUE){
							$$.numeric=TRUE;
							$$.ttype=INT;
							if(($1.ttype==INT && $4.ttype ==INT) || ($1.ttype == FLOAT && $4.ttype==FLOAT)) {$$.ttype=INT;
								if(founderror==FALSE){
									switch($<value.relopvalue>2){
                                        case LES:       if($1.ttype==INT) gen_instr("ltI");
                                                                else if($1.ttype==FLOAT) gen_instr("ltR");
                                                                            break;
                                        case LEQ:       if($1.ttype==INT) gen_instr("leI");
                                                                else if($1.ttype==FLOAT) gen_instr("leR");
                                                                            break;
										case GRE:	if($1.ttype==INT) gen_instr("gtI");
												else if($1.ttype==FLOAT) gen_instr("gtR");
												break;
										case GEQ:	if($1.ttype==INT) gen_instr("geI");
												else if($1.ttype==FLOAT) gen_instr("geR");
												break;
                                        default:    break;
                                                                        }
								}

							}
							else if($1.ttype ==INT && $4.ttype==FLOAT){
								warning("expressons are of different type, data may be lost","");
								$$.ttype = INT;
								if(founderror==FALSE){
                                                switch($<value.relopvalue>2){
                                                        case LES:       gen_instr("fltb");
												gen_instr("ltR");
                                                                                                break;
                                                        case LEQ:       gen_instr("fltb");
												gen_instr("leR");
                                                                                                break;
                                                        case GRE:       gen_instr("fltb");
												gen_instr("gtR");
                                                                                                break;
                                                        case GEQ:       gen_instr("fltb");
												gen_instr("geR");
                                                                                                break;
                                                        default:    break;
                                                                        }
                                                                }
							}
							else if($1.ttype ==FLOAT && $4.ttype ==INT) {
								warning("expression are of different type, data may be lost","");
								$$.ttype = INT;
								if(founderror==FALSE){
                                                                        switch($<value.relopvalue>2){
                                                                                case LES:       gen_instr("flt");
                                                                                                gen_instr("ltR");
                                                                                                break;
                                                                                case LEQ:       gen_instr("flt");
                                                                                                gen_instr("leR");
                                                                                                break;
                                                                                case GRE:       gen_instr("flt");
                                                                                                gen_instr("gtR");
                                                                                                break;
                                                                                case GEQ:       gen_instr("flt");
                                                                                                gen_instr("geR");
                                                                                                break;
                                                                                default:     break;
                                                                        }
                                                                }
							}

						}
						else{
							error("non numeric in operation","");
							$$.numeric=FALSE;
						}
					}
	| simpleexpr { $$.lval = $1.lval; $$.ttype = $1.ttype; $$.numeric=$1.numeric;}
	| simpleexpr relop error { yyerrok; error("(unexpected token after relational operator","");}
;

simpleexpr: simpleexpr
	addop {
		if(founderror==FALSE){
                        if($1.numeric==TRUE){
                                switch($1.ttype){
                                        case INT:       if($1.lval==TRUE) gen_instr("fetchI"); break;
                                        case FLOAT:     if($1.lval==TRUE) gen_instr("fetchR"); break;
                                        default:        break;
                                }
                        }
                }

	}

	term {
			if(founderror==FALSE){
                                if($4.numeric ==TRUE){
                                        switch($4.ttype){
                                                case INT:       if($4.lval==TRUE) gen_instr("fetchI"); break;
                                                case FLOAT:     if($4.lval==TRUE) gen_instr("fetchR"); break;
                                                default:        break;
                                        }
                                }
                        }
						$$.lval = FALSE;
						if($1.numeric == TRUE && $4.numeric==TRUE){
							$$.numeric=TRUE;
							if(($1.ttype==INT && $4.ttype ==INT) || ($1.ttype == FLOAT && $4.ttype==FLOAT)) {$$.ttype=$1.ttype;
								if(founderror==FALSE){
        		                                                switch($<value.addopvalue>2){
                		                                                case PLS:       if($1.ttype==INT) gen_instr("addI");
                        		                                                        else if($1.ttype==FLOAT) gen_instr("addR");
                                		                                                break;
                                        		                        case MIN:       if($1.ttype==INT) gen_instr("subI");
                                                		                                else if($1.ttype==FLOAT) gen_instr("subR");
                                                        		                        break;
                                                                        default:        break;
                           	                       		      	}
	                                	            	}
							}
							else if($1.ttype ==INT && $4.ttype==FLOAT){ warning("expressons are of different type, data may be lost","");
								$$.ttype = FLOAT;
		                                                if(founderror==FALSE){
		                                                        switch($<value.addopvalue>2){
		                                                                case PLS:       gen_instr("fltb");
		                                                                                gen_instr("addR");
		                                                                                break;
		                                                                case MIN:       gen_instr("fltb");
		                                                                                gen_instr("subR");
		                                                                                break;
                                                                        default:        break;
		                                                        }
		                                                }
							}
							else if($1.ttype ==FLOAT && $4.ttype ==INT) { warning("expression are of different type, data may be lost","");
								$$.ttype = FLOAT;
                        		                        if(founderror==FALSE){
                        		                                switch($<value.addopvalue>2){
                        		                                        case PLS:       gen_instr("flt");
                        		                                                        gen_instr("addR");
                        		                                                        break;
                        		                                        case MIN:       gen_instr("flt");
                        		                                                        gen_instr("subR");
                		                                                                break;
                                                                        default:                break;
                		                                        }
		                                                }
							}
						}
						else{
							error("non numeric in operation","");
							$$.numeric=FALSE;
						}

	}
	| term {$$.lval = $1.lval; $$.ttype = $1.ttype; $$.numeric = $1.numeric;}
	| simpleexpr addop error { yyerrok; error("(unexpected token after additive operator)","");}
;

term: term mulop {
		if(founderror==FALSE){
			if($1.numeric==TRUE){
				switch($1.ttype){
					case INT:	if($1.lval==TRUE) gen_instr("fetchI"); break;
					case FLOAT:	if($1.lval==TRUE) gen_instr("fetchR"); break;
                    default:    break;
				}
			}
		}
	}
	factor {
			if(founderror==FALSE){
				if($4.numeric ==TRUE){
					switch($4.ttype){
						case INT:	if($4.lval==TRUE) gen_instr("fetchI"); break;
						case FLOAT:	if($4.lval==TRUE) gen_instr("fetchR"); break;
                        default:    break;
					}
				}
			}
				$$.lval = FALSE;
				if($1.numeric ==TRUE && $4.numeric ==TRUE){
					$$.numeric =TRUE;
					if(($1.ttype==INT && $4.ttype ==INT) || ($1.ttype == FLOAT && $4.ttype==FLOAT)) {$$.ttype = $1.ttype;
						if(founderror==FALSE){
							switch($<value.multopvalue>2){
								case DIV:	if($1.ttype==INT) gen_instr("divI");
										else if($1.ttype==FLOAT) gen_instr("divR");
										break;
								case MULT:	if($1.ttype==INT) gen_instr("mulI");
										else if($1.ttype==FLOAT) gen_instr("mulR");
										break;
                                        default:    break;
							}
						}
					}
					else if($1.ttype ==INT && $4.ttype==FLOAT){
						warning("expressons are of different type, data may be lost","");
						$$.ttype = FLOAT;
						if(founderror==FALSE){
							switch($<value.multopvalue>2){
        	                                                case DIV:       gen_instr("fltb");
										gen_instr("divR");
                        	                                                break;
                                	                        case MULT:      gen_instr("fltb");
										gen_instr("mulR");
                                                	                        break;
                                                            default:    break;
	                                                }
						}
					}
					else if($1.ttype ==FLOAT && $4.ttype ==INT) {
						warning("expression are of different type, data may be lost","");
						$$.ttype = FLOAT;
						if(founderror==FALSE){
							switch($<value.multopvalue>2){
        	                                                case DIV:       gen_instr("flt");
                	                                                        gen_instr("divR");
                        	                                                break;
                                	                        case MULT:      gen_instr("flt");
                                        	                                gen_instr("mulR");
                                                	                        break;
                                                            default:            break;
	                                                }
						}
					}

				}
				else{
					error("non numeric in operation","");
					$$.numeric =FALSE;
				}
			}
	| factor {$$.lval = $1.lval; $$.ttype = $1.ttype; $$ = $1; $$.numeric=$1.numeric;}
	| term mulop error { yyerrok; error("(unexpected token after multiplicative operator)","");
				}
;

factor: constant { $$.ttype = $1.ttype; $$.lval = FALSE; $$.numeric=TRUE;
		if(founderror==FALSE){
			if($$.ttype ==INT)
				gen_instr_I("pushcI",$<value.ivalue>1);
			else if($$.ttype == FLOAT)
				gen_instr_F("pushcR",$<value.fvalue>1);
			else if($$.ttype == STR){
				gen_instr_S("pushs",$<value.svalue>1);
				$$.numeric=FALSE;
			}
			else
				error("constant is not a correct type of constant","");
		}
	}
	| Ident { Entry *tempE, *tempE2; $<value.svalue>$ = $1;
				if(strcmp((char*)$<value.svalue>1, "main")!=0){
					//if(lookup((char*)$<value.svalue>1, mysymtab) == NULL)
					//	error("variable undeclared, please declare variables before using them","");
					if(1){
						tempE2 = (Entry*) malloc(sizeof(Entry));
						tempE2->name =(char*) strdup($<value.svalue>1);
						#ifdef DEBUG
						fprintf(stderr,"the name of the identifier here is:  %s\n", (char*)$<value.svalue>1);
						#endif
						if((tempE=lookupB((char*)tempE2->name,mysymtab)) !=NULL){

							if(tempE->self ==VAR || tempE->self == PARAM){
								switch(tempE->self){
									case VAR:
										$$.ttype = ((Varb*)(tempE->binding))->type;
										#ifdef DEBUG
										char temp_char = (char)$$.ttype;
										if($$.ttype !=  NULL) fprintf(stderr,"type is: %s\n", &temp_char);
										if(((Varb*)(tempE->binding))->type !=NULL) fprintf(stderr,"type is: %d\n", ((Varb*)(tempE->binding))->type);
										#endif
										$$.lval = TRUE;
										if(((Varb*)(tempE->binding))->type == INT || ((Varb*)(tempE->binding))->type ==FLOAT) $$.numeric=TRUE;
										if(founderror==FALSE){
											if(inCscope(tempE2->name, mysymtab) == TRUE)
												gen_instr_I("pusha", ((Varb*)(tempE->binding))->offset);
											else{
												gen_instr_tI("pushga",getleveldif(tempE2->name,mysymtab),((Varb*)(tempE->binding))->offset);
												//do something else

											}
										}
										break;
									case PARAM:
										$$.ttype = ((Paramb*)(tempE->binding))->type;
										#ifdef DEBUG
										fprintf(stderr,"type is: %d\n", (int)$$.ttype);
										#endif
										$$.lval = TRUE;
										if(((Paramb*)(tempE->binding))->type == INT || ((Paramb*)(tempE->binding))->type ==FLOAT) $$.numeric=TRUE;
										if(founderror==FALSE){
											if(inCscope(tempE2->name,mysymtab) ==TRUE){
												gen_instr_I("pusha", ((Varb*)(tempE->binding))->offset);
											}
											else{
												//do something else
											}
										}
										break;
                                    default:        break;

								}
							}
							else
								error("Variable is unknown or undelcared","");
						}
						else{
							$$.lval=FALSE;
							$$.numeric=FALSE;
							$$.ttype=VOID;
							error("Variable is unknown or undelcared, couldn't find in symbol table'","");
						}
					}
				}
				else{
					error("Main is not a variable name","");
						//tempE2 = (Entry*) malloc(sizeof(Entry));
						//tempE2->name = (char*) $<value.svalue>1;

				}
		}
	| lpar expr rpar { $$.lval = $2->lval; $$.ttype = $2->type; $$.numeric= $2->numeric;

				}
	| addop factor %prec uminus {
				if(founderror==FALSE){
					if($2.numeric==TRUE){
						if($1 == MIN){
							if($2.lval ==TRUE){
								switch($2.ttype){
									case INT:	gen_instr("fetchI"); break;
									case FLOAT:	gen_instr("fetchR"); break;
                                    default:    break;
								}
							}
							switch($2.ttype){
								case INT:	gen_instr("negI"); break;
								case FLOAT:	gen_instr("negR"); break;
                                default:    break;
							}
						}
						else if($1 == PLS){
							if($2.lval == TRUE){
								switch($2.ttype){
									case INT:	gen_instr("fetchI"); break;
									case FLOAT:	gen_instr("fetchR"); break;
                                    default:    break;
								}
							}
						}
					}
				}
						$$.lval = FALSE;
						$$.ttype = $2.ttype;
						if($2.numeric ==TRUE) $$.numeric=TRUE;
						else
							error("cannot change sign of non numeric expression","");
					}
	| adof Ident {
				Entry *tempE, *tempE2; $<value.svalue>$ = $2;
                                if(strcmp((char*)$<value.svalue>2, "main")!=0){
                                        if(lookup((char*)$<value.svalue>2, mysymtab) == NULL)
										error("variable undeclared, please declare variables before using them","");
                                        else{
                                                tempE2 = (Entry*) malloc(sizeof(Entry));
                                                tempE2->name =(char*) $<value.svalue>2;
                        if((tempE=lookupB((char*)$<value.svalue>2,mysymtab)) !=NULL){

                            if(tempE->self ==VAR || tempE->self == PARAM){
                                switch(tempE->self){
                                    case VAR:
                                        $$.ttype = ((Varb*)(tempE->binding))->type;
                                        #ifdef DEBUG
                                        fprintf(stderr,"type is: %d\n", (int)$$.ttype);
                                        #endif
                                        $$.lval = FALSE;
                                        if(((Varb*)(tempE->binding))->type == INT || ((Varb*)(tempE->binding))->type ==FLOAT)
                                            $$.numeric=TRUE;
										if(founderror==FALSE){
                                            if(inCscope((char*)strdup($<value.svalue>2), mysymtab) == TRUE)
                                                gen_instr_I("pusha", ((Varb*)(tempE->binding))->offset);
                                            else{
                                                gen_instr_tI("pushga",getleveldif($<value.svalue>2,mysymtab),((Varb*)(tempE->binding))->offset);
                                    //do something else

                                            }
                                        }
                                        break;
                                    case PARAM:
                                        $$.ttype = ((Paramb*)(tempE->binding))->type;
                                        #ifdef DEBUG
                                        fprintf(stderr,"type is: %d\n", (int)$$.ttype);
                                        #endif
                                        $$.lval = FALSE;
                                        if(((Paramb*)(tempE->binding))->type == INT || ((Paramb*)(tempE->binding))->type ==FLOAT)
                                            $$.numeric=TRUE;
                                        if(founderror==FALSE){
                                            if(inCscope((char*)$<value.svalue>2,mysymtab) ==TRUE){
                                                gen_instr_I("pusha", ((Varb*)(tempE->binding))->offset);
                                            }
                                            else{
                                //do something else
                                            }
                                        }
                                        break;
                                    default:            break;
                                }
                                                        }
                                                        else
								error("Variable is unknown or undelcared", "");
                                                }
                                                else{
                                                        $$.lval=FALSE;
                                                        $$.numeric=FALSE;
                                                        $$.ttype=VOID;
                                                        error("Variable is unknown or undelcared","");
                                                }
                                        }
                                }
                                else{
                                        error("Main is not a variable name", "");
                                                //tempE2 = (Entry*) malloc(sizeof(Entry));
                                                //tempE2->name = (char*) $<value.svalue>1;

                                }
			}
	| function_call { $$.ttype = $1.ttype; $$.lval = FALSE; $$.numeric=$1.numeric; }

	| addop error { yyerrok; error("(unexpected token after unary additive operator)","");

					}
//	| Ident lpar exprlist error { yyerrok; error("(unexpected token token before rpar)","");
//					}
	| lpar expr error rpar { yyerrok; error("(unexpected token before rpar)","");

					}

;

function_call: Ident lpar rpar {$$.lval = FALSE; Funcb* tempb; Entry* tempE; Entry *tempE2;
                                if((tempb=lookup((char*)$<value.svalue>1,mysymtab)) == NULL){
                                        error("function undeclared, please declare functions before using them","");
				}
                                else{
                                        tempE2 = (Entry*) malloc(sizeof(Entry));
                                        tempE2->name = (char*)$<value.svalue>1;
                                        if((tempE=lookupB((char*)$<value.svalue>1,mysymtab))!=NULL){
                                                if(tempE->self ==FUNC){
							if(tempb->returntype !=VOID) $$.lval =TRUE; else $$.lval=FALSE;
                                                        if(tempb->num_param != 0)
                                                                error("Function call without correct number of parameters if any","");
                                                        $$.ttype = tempb->returntype;
                                                        if($$.ttype == INT || $$.ttype ==FLOAT) $$.numeric =TRUE; else $$.numeric =FALSE;
							if(founderror==FALSE){
								gen_instr_I("enter",1);
								gen_call(genlabelw($<value.svalue>1, tempb->label), 0);
							}
                                                }
                                                else
                                                        error("Function call with an unknown function name", "");
                                        }
                                        else
                                                error("fuction undeclared","");
                                        free(tempE2); tempE2=NULL;
                                }
                        }
	| func_call_with_params {$$.ttype =$1.ttype; $$.numeric = $1.numeric; $$.lval = $1.lval;}
;

func_call_with_params: name_and_params rpar{$$.numeric =$1.numeric; $$.lval = FALSE; $$.ttype = $1.ttype;
				if($1.funcent!=NULL){
						if(($1.funcent)->self == FUNC){
							if( ((Funcb*)(($1.funcent)->binding))->returntype != VOID) $$.numeric = TRUE; else $$.numeric=FALSE;
						}
						if(founderror==FALSE){
							if(strcmp("scanf",$1.funcent->name)==0){
//								gen_call("$scanf",((Funcb*)($1.funcent->binding))->actual_num);
								gen_call("$scanf",$1.params);
							}
							else if(strcmp("printf", $1.funcent->name)==0){
//								gen_call("$printf",((Funcb*)($1.funcent->binding))->actual_num);
								gen_call("$printf",$1.params);
							}
							else{
								if( ((Funcb*)($1.funcent->binding))->label==0) ((Funcb*)($1.funcent->binding))->label=getlabel();
								gen_call( genlabelw($1.funcent->name,((Funcb*)($1.funcent->binding))->label),
									((Funcb*)($1.funcent->binding))->num_param);
							}
						}
				}
			}
;

name_and_params:
       Ident
	lpar
	{
		$$.funcent =NULL;
		$$.funcent = lookupB((char*)$<value.svalue>1,mysymtab);
		#ifdef DEBUG
		printTree(mysymtab);
		fprintf(stderr,"this the name of function called and the lookup value: %s\n",$1);
		if(lookupB($1,mysymtab)==NULL) fprintf(stderr,"it was null\n");
		else fprintf(stderr,"wasn't null\n");
		#endif
		if ($$.funcent != NULL){
			$$.name = $$.funcent->name;
			if(founderror==FALSE) gen_instr_I("enter",1);
		}
	}
	expr {
        Entry *tempE, *tempE2;
        $$.lval = FALSE;
        //listnodeE* tempexprN;
        //ListE * tempLE;
        //int a;
        Funcb* tempB;
        if((tempB=lookup((char*)$<value.svalue>1,mysymtab)) ==NULL){
            error("function undelcared, please declare functions before using them","");
            error("1","");
            $$.funcent=NULL;
        }
        else {
					//warning("just checking value of entry: %s",$<value.funcentvalue>$->name);
            tempE2 = (Entry*) malloc(sizeof(Entry));
            tempE2->name = (char*) $<value.svalue>1;
            if( (tempE=lookupB((char*)$<value.svalue>1,mysymtab))!=NULL){
                if(tempE->self != FUNC){
                    error("function undeclared, please declare functions before using them", "");
                    error("2","");
                    $$.funcent=NULL;
                }
                else{
                    if(tempB->num_param ==0){
                        error("Paramter given for a function that takes no parameters.","");
                    }
                    else if(tempB->num_param == -1){
                        #ifdef DEBUG
                        fprintf(stderr,"SPRINTF OR PRINTF mismatch: FUNCTION NAME: %s\n",$1);
                        fprintf(stderr,"SPRINTF OR PRINTF: FUNCTION TYPE: %d\n",(int)$4->type);
//                                                            fprintf(stderr,"Function mismatch 1: FUNCTION NAME: %s\n",$1);
  //                                                          fprintf(stderr,"Function mismatch 1: FUNCTION NAME: %s\n",$1);
                        fprintf(stderr,"SPRINTF OR PRINTF: $4 TYPE: %d\n",(int)$4->type);
                        fprintf(stderr,"SPRINTF OR PRINTF: tempB->param_type[0] TYPE: %d\n",(int)tempB->param_type[0]);
                        #endif
                        if($4->type != tempB->param_type[0]){
                            error("parameter type is different in declaration and in function call","");
                        }
                        else{
                            if(founderror==FALSE){
										//gen_instr_S("pushs",$<value.svalue>4);
                            }
                        }
                        $$.ttype = tempB->param_type[0];
                        if($$.ttype==INT || $$.ttype ==FLOAT)
                            $$.numeric=TRUE;
                        else
                            $$.numeric=FALSE;
                        $$.funcent=$3.funcent;
						$$.params = 1;
                    }
                    else{
                        if($4->lval==TRUE && $4->numeric==TRUE){
                            if(founderror==FALSE){
                                switch($4->type){
                                    case INT:	gen_instr("fetchI"); break;
                                    case FLOAT:	gen_instr("fetchR"); break;
                                    default:    break;
                                }
                            }
                        }
                        if(tempB->param_type !=NULL){
                            if($4->type != tempB->param_type[0]){
                                #ifdef DEBUG
                                fprintf(stderr,"Function mismatch 2: FUNCTION NAME: %s\n",$1);
                                fprintf(stderr,"Function mismatch 2: FUNCTION TYPE: %d\n",(int)$4->type);
                                            //                                                            fprintf(stderr,"Function mismatch 1: FUNCTION NAME: %s\n",$1);
                                            //                                                          fprintf(stderr,"Function mismatch 1: FUNCTION NAME: %s\n",$1);
                                #endif
                                if(tempB->param_type[0]!=INT && tempB->param_type[0]!=FLOAT)
                                    error("Parameter type is different in declaration and in function call","");
                                else if(tempB->param_type[0]==INT){
                                        #ifdef DEBUG
                                        fprintf(stderr,"Function mismatch 3: FUNCTION NAME: %s\n",$1);
                                        fprintf(stderr,"Function mismatch 3: FUNCTION TYPE: %d\n",(int)$4->type);
                                                //                                                            fprintf(stderr,"Function mismatch 1: FUNCTION NAME: %s\n",$1);
                                                //                                                          fprintf(stderr,"Function mismatch 1: FUNCTION NAME: %s\n",$1);
                                        #endif
                                        switch($4->type){
                                            case FLOAT:
                                                warning("Paramter expression will lose data because of different type","");
                                                if(founderror==FALSE)
                                                    gen_instr("int");
                                                break;
                                            case INT:	break;
                                            default:	error("Parameter type is different in declaration and function call","");
                                                        break;
                                        }
                                }
                                else if(tempB->param_type[0]==FLOAT){
                                    #ifdef DEBUG
                                    fprintf(stderr,"Function mismatch 4: FUNCTION NAME: %s\n",$1);
                                    #endif
                                    switch($4->type){
                                        case INT:	warning("Parameter expression is different type than in declaration","");
                                                    if(founderror==FALSE)
                                                        gen_instr("flt");
                                                    break;
                                        case FLOAT:	break;
                                        default:	error("Parameter type is different in declaration and function call","");
                                                            break;
                                    }
                                }

                            }
                        }
                        $$.funcent=$3.funcent;

                        if(tempB->param_type !=NULL)
                            $$.ttype=tempB->param_type[0];
                        if($$.ttype==INT || $$.ttype==FLOAT)
                            $$.numeric =TRUE;
                        else
                            $$.numeric=FALSE;
                        $$.params=1;
                    }
                }
            }
            else
                error("Function is undeclared","");
            free(tempE2); tempE2=NULL;

        }
    }
	| name_and_params
	comma {}
	expr {
			Entry *tempE, *tempE2;
            $$.lval = FALSE;
            //listnodeE* tempexprN;
            //ListE * tempLE;
            //int a;
            Funcb* tempB;
            if($1.funcent==NULL){
            	error("function undelcared, please declare functions before using them","");
			   error("3","");
			}
            else {
				$$.funcent = $1.funcent;
                tempE2 = (Entry*) malloc(sizeof(Entry));
                tempE2->name = (char*) $1.funcent->name;
				tempB=lookup((char*)$1.funcent->name,mysymtab);
                if( (tempE=lookupB((char*)$1.funcent->name,mysymtab))!=NULL){
                	if(tempE->self != FUNC){
                    	error("function undeclared, please declare functions before using them", "");
						error("4","");
					}
                    else{
                    	if(tempB->num_param ==0){
						}
 					    else if(tempB->num_param == -1){
                        	$$.ttype = tempB->param_type[$1.params];
                            if($$.ttype==INT || $$.ttype ==FLOAT) $$.numeric=TRUE; else $$.numeric=FALSE;
								$$.params = $1.params +1;
								$$.funcent=$1.funcent;
								if($4->lval==TRUE && $4->numeric==TRUE && strcmp("scanf",$$.funcent->name)!=0){
									if(founderror==FALSE){
										switch($4->type){
											case INT:	gen_instr("fetchI"); break;
											case FLOAT:	gen_instr("fetchR"); break;
                                            default:    break;
										}
									}
								}
								
						}
                        else if($$.params < tempB->num_param){
                        	if($4->type != tempB->param_type[$1.params]){
                            	#ifdef DEBUG
                                fprintf(stderr,"Function mismatch before warning: FUNCTION NAME: %s\n",$1.name);
                                #endif

                                warning("Parameter type is different in declaration and in function call","");
								if(founderror==FALSE){
									if($4->lval==TRUE){
										switch($4->type){
											case INT:	gen_instr("fetchI"); break;
											case FLOAT:	gen_instr("fetchR"); break;
                                            default:    break;
										}
									}
									if(tempB->param_type[$1.params]==FLOAT){
										gen_instr("flt");
									}
									else if(tempB->param_type[$1.params]==INT){
										gen_instr("int");
									}
								}
							}
							else{
								if(founderror==FALSE){
									if($4->lval==TRUE){
										switch($4->type){
											case INT:	gen_instr("fetchI"); break;
											case FLOAT:	gen_instr("fetchR"); break;
                                            default:    break;
										}
									}
								}

							}
                            $$.ttype=tempB->param_type[$1.params];
                            if($$.ttype==INT || $$.ttype==FLOAT) $$.numeric =TRUE; else $$.numeric=FALSE;
							$$.params=$1.params +1;
							$$.funcent=$1.funcent;
                      }
					  else{
					  	error("Too many parameters given for function in function call.","");
					  }
				  }
			   }
			   else
               	error("Function is undeclared","");
               free(tempE2); tempE2=NULL;

		   }
	   }

	//| Ident lpar expr error { yyerrok; error("(unexpected token token before rpar)","");}
	//| Ident lpar name_and_params comma expr error { yyerrok; error("(unexpected token token before rpar)","");}
	//| name_and_params comma error Ident { yyerrok; error("(unexpected token after comma)","");}


;

constant: StrConstant { $<value.svalue>$ = $1;
				$$.ttype = STR;
				$$.lval = FALSE;
				$$.numeric=FALSE;
				}
	| IntConstant {$<value.ivalue>$ = $1;
				$$.ttype = INT;
				$$.lval = FALSE;
				$$.numeric= TRUE;
				}
	| FloatConstant { $<value.fvalue>$ = $1;
				$$.ttype = FLOAT;
				$$.lval = FALSE;
				$$.numeric= TRUE;
				}
;


identlist: Ident { $$ = mklist(((char*)$1));}
	| identlist comma Ident { $$ = appendList( $1, $3);}
	| identlist comma error {yyerrok; error("(unexpected token after comma)","");
						//deleteList($1);

				}
;


addop: plus { $$ = $1;}
	| minus { $$ = $1;}
;

mulop: star { $$ = $1;}
	| divide {$$ = $1;}
;

eqop: equequ { $$ = $1;}
	| neq { $$ = $1;}
;

relop: lesst { $$ = $1;}
	| leq { $$ = $1;}
	| geq { $$ = $1;}
	| greatt { $$ = $1;}
;


%%

int yyerror(char *s)
{
//	fprintf(stderr,"Error: Line: %d: %s\n", Line_Number, s);
	fprintf(stderr,"Error:::%s:%d-> %s\n",filename,Line_Number,s);
    return 0;
}


int warning(char *s1, char* s2)
{
//	fprintf(stderr,"Warning: Line: %d: %s %s\n",Line_Number,s1,s2);
	fprintf(stderr,":::::Warning:::%s:%d-> %s %s\n",filename,Line_Number,s1,s2);
    return 0;
}

int error(char* s1, char* s2){
//	fprintf(stderr,"Error: Line: %d: %s %s\n",Line_Number,s1,s2);
	fprintf(stderr,"Error:::%s:%d-> %s %s\n",filename,Line_Number,s1, s2);
	founderror=TRUE;
    return 0;
}

