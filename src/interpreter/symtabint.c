#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "symtabint.h"
#include "typeint.h"
#include "Listint.h"
#include "dataint.h"
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include "interpreter.l.h"
#include "debuglib.h"
#include <string.h>

int offset_counter;
//extern int error(char*,char*);

void openscope(Symtab *symtab){
	if(symtab->actualStacksize == symtab->Stacksize)
		error(0,"Scope Stack already too full",0);
	else{
		#ifdef DEBUG
		debugprint(SYMTABC,"\nOpening new Scope","");
		#endif
	 	symtab->actualStacksize += 1;

		offset_counter=5;
	}
}

void closescope(Symtab *symtab){
	Entry * temp;
	if(symtab->actualStacksize == 1)
		error(0,"Cannot close Global scope","");
	else{
		#ifdef DEBUG
		debugprint(SYMTABC,"\nClosing Scope","");
		#endif
		if((symtab->Stack[symtab->actualStacksize-1]) !=NULL){
			while(symtab->Stack[symtab->actualStacksize-1] != NULL){
				temp = *((Entry**)(symtab->Stack[symtab->actualStacksize-1] ));
				tdelete((void*)temp,(void **) &(symtab->Stack[symtab->actualStacksize-1]), Ecmp);
				deleteEntry(temp);

			}
			symtab->Stack[symtab->actualStacksize-1]=NULL;
		}
			symtab->actualStacksize -= 1;
	}
}

void * lookup(const char * name, Symtab *symtab){
	Entry ** found;
    found = NULL;
	Entry *temp;
	int a;
	if(name != NULL){
		temp = (Entry*) malloc(sizeof(Entry));
		temp->name = (char*) strdup((char*)name);
		for(a=symtab->actualStacksize-1;a>=0;a--){
			found = (Entry**) tfind((void*) temp, (void**) &(symtab->Stack[a]),Ecmp);
			if(found != NULL){
				if(*found !=NULL) break;
			}
		}
		if(found != NULL){
			if(*found ==NULL){
				free(temp->name);
				free(temp);
				return NULL;
			}
			else{
				free(temp);
				return ((void*) (*found)->binding);
			}
		}
		else{
			free(temp->name);
			free(temp);
			return NULL;
		}

	}
	else{
		error(0,"cannot lookup variable without a name","");
		return NULL;
	}
}

void install(Entry * temp,Symtab *symtab){
	Entry ** found;
	found = (Entry**) tsearch((void*) temp, (void**) &((symtab->Stack[symtab->actualStacksize-1])),Ecmp);

	if(*found !=temp)
		error(0,"symbol already declared in current scope","");
}

int Ecmp(const void *Entry1, const void *Entry2){
	return strcmp( ((Entry*)Entry1)->name, ((Entry*)Entry2)->name);
}

#ifdef DEBUG
void printTree(Symtab *symtab){
	twalk((void*) (symtab->Stack[symtab->actualStacksize-1]), Swalk);
}

void Swalk(const void *node, VISIT myorder, int level){
		Entry * temp;
		Funcbcg * tempb;
		int a=0;
	if(myorder == preorder || myorder == leaf){
		temp = (*(Entry**) node);
		tempb = ((Funcbcg*)(temp->binding));
		switch(temp->self){

		case(FUNC):		debugprint(SYMTABC,"Name: %s\nReturn Type: ", temp->name);
					switch(tempb->returntype){
						case(INT):	debugprint(SYMTABC,"INT",""); break;
						case(FLOAT):	debugprint(SYMTABC,"FLOAT",""); break;
						case(VOID):	debugprint(SYMTABC,"VOID",""); break;
                        case CHAR:

                            break;
                        case STR:

                            break;
                        case REFINT:

                            break;
                        case REFFLOAT:

                            break;
                        case REFSTR:

                            break;
			default:
				break;
                    }
					dbprint(0,"Body Defined: ",  1, INT, ((int)tempb->bodydef));
					dbprint(0,"Number Parameters: ", 1,INT,tempb->num_param);
					if(tempb->num_param >0){
						debugprint(SYMTABC,"Parameter Types: ","");
						for(a=0;a<tempb->num_param;a++){
							switch(tempb->param_type[a]){
								case(INT):	debugprint(SYMTABC,"INT ","");break;
								case(FLOAT):	debugprint(SYMTABC,"FLOAT ",""); break;
								case(REFINT):	debugprint(SYMTABC,"REFINT ",""); break;
								case(REFFLOAT):	debugprint(SYMTABC,"REFFLOAT ",""); break;
								case(REFSTR):	debugprint(SYMTABC,"REFSTR ",""); break;
								case(STR):	debugprint(SYMTABC,"STR ",""); break;
								case(VOID):	debugprint(SYMTABC,"VOID ",""); break;
				                                case (CHAR):	debugprint(SYMTABC,"CHAR ",""); break;
								default: break;
                            }
						}
					}
					if(tempb->num_param == -1){
						debugprint(SYMTABC,"Parameter Types: ","");
                        for(a=0;a<tempb->actual_num;a++){
                            switch(tempb->param_type[a]){
                                                                case(INT):      debugprint(SYMTABC,"INT ","");break;
                                                                case(FLOAT):    debugprint(SYMTABC,"FLOAT ",""); break;
                                                                case(REFINT):   debugprint(SYMTABC,"REFINT ",""); break;
                                                                case(REFFLOAT): debugprint(SYMTABC,"REFFLOAT ",""); break;
                                                                case(REFSTR):   debugprint(SYMTABC,"REFSTR ",""); break;
                                                                case(STR):      debugprint(SYMTABC,"STR ",""); break;
                                                                case(VOID):     debugprint(SYMTABC,"VOID ",""); break;
 				                               case (CHAR):	debugprint(SYMTABC,"CHAR ",""); break;
								default:	break;
                            }
                        }
						//fprintf(stderr,"...");

					}
					debugprint(SYMTABC,"\n","");

					break;
		case(PARAM):		debugprint(SYMTABC,"Name: %s\nType: ", temp->name);
					switch( ((Parambcg*)(temp->binding))->type){
						case(INT):	debugprint(SYMTABC,"INT",""); break;
						case(FLOAT):	debugprint(SYMTABC,"FLOAT",""); break;
                        case VOID:

                            break;
                        case CHAR:

                            break;
                        case(STR):	debugprint(SYMTABC,"STR",""); break;
						case(REFINT):	debugprint(SYMTABC,"REFINT",""); break;
						case(REFFLOAT):	debugprint(SYMTABC,"REFFLOAT",""); break;
						case(REFSTR):	debugprint(SYMTABC,"REFSTR",""); break;
						default:	break;
					}
					dbprint(0,"Offset: ", 1, INT, ((Parambcg*)(temp->binding))->offset);

					break;
		case(VAR):		debugprint(SYMTABC,"Name: %s\nType: ", temp->name);
					switch(((Varbcg*)(temp->binding))->type){
						case(INT):	debugprint(SYMTABC,"INT","");
								break;
						case(FLOAT):	debugprint(SYMTABC,"FLOAT","");

                        case VOID:

                            break;
                        case CHAR:

                            break;
                        case STR:

                            break;
                        case REFINT:

                            break;
                        case REFFLOAT:

                            break;
                        case REFSTR:

                            break;
			default:	break;
                    }
					dbprint(0,"Offset: ", 1, INT, ((Varbcg*)(temp->binding))->offset);

					break;

		}
	}
}
#endif
Symtab * createTree(int Stacksize){
	Symtab *temp;
	int a;
	temp = (Symtab*) malloc(sizeof(Symtab));
	temp->actualStacksize=1;
	temp->Stacksize = Stacksize;
	temp->Stack = (Entry**) malloc((sizeof(Entry*) * temp->Stacksize));
	for(a=0;a<Stacksize;a++) temp->Stack[a]=NULL;
	return temp;
}

void deleteTree(Symtab *symtab){
	Entry * temp;
	//Entry ** found;
	if(symtab != NULL){
	   #ifdef DEBUG
	   debugprint(SYMTABC,"Deleting Tree:","");
	   #endif
	    while(symtab->actualStacksize != 1)
		closescope(symtab);
	    if((symtab->Stack[symtab->actualStacksize-1]) !=NULL){
                        while(symtab->Stack[symtab->actualStacksize-1] != NULL){
                                temp = *((Entry**)(symtab->Stack[symtab->actualStacksize-1] ));
                                tdelete((void*)temp,(void **) &(symtab->Stack[symtab->actualStacksize-1]), Ecmp);
                                deleteEntry(temp);

                        }
                        symtab->Stack[symtab->actualStacksize-1]=NULL;
            }
	    #ifdef DEBUG
	    debugprint(SYMTABC,"Deleting Stack","");
	    #endif
	    free(symtab->Stack);
	    #ifdef DEBUG
	    debugprint(SYMTABC,"Deleting Symbol Table","");
	    #endif
	    free(symtab);
	    symtab=NULL;
	}
}
void deleteEntry(Entry * temp){
	if(temp != NULL){
		switch(temp->self){

			case(FUNC):
						#ifdef DEBUG
						debugprint(SYMTABC,"Deleting Function ", temp->name);
						#endif
						if( ((Funcbcg*)(temp->binding))->num_param > 0 || ((Funcbcg*)(temp->binding))->num_param == -1)
							free( ((Funcbcg*)(temp->binding))->param_type);
						free(temp->binding);
						free(temp->name);
						free(temp);
						temp=NULL;
						break;
			case(VAR):
						#ifdef DEBUG
						debugprint(SYMTABC,"Deleting variable ", temp->name);
						#endif
						free(temp->name);
						free(temp->binding);
						free(temp);
						temp=NULL;
						break;
			case(PARAM):
						#ifdef DEBUG
						debugprint(SYMTABC,"Deleteing Parameter ", temp->name);
						#endif
						free(temp->name);
						free(temp->binding);
						free(temp);
						temp= NULL;
						break;
			default:		error(0,"Error in Node, doesn't have correct binding","");
		}
	}

}


Entry *createFunc(char * name, typecg returntype, ListP* paramlist){
	Entry * temp;
	listnodeP * tempP;
	int a;
	boolcg elip=FALSE;
	temp = (Entry*) malloc(sizeof(Entry));
	if(name !=NULL){
		temp->name = (char*)strdup(((const char*)name));
		temp->self = FUNC;
		//Funcb * tBinding = (Funcb*)temp->binding;
        Funcbcg * tBinding = NULL;
		//((Funcb*)(temp->binding)) = (Funcb*) malloc(sizeof(Funcb));
		//((Funcb*)(temp->binding))->returntype = returntype;
		tBinding = (Funcbcg*) malloc(sizeof(Funcbcg));
		tBinding->returntype = returntype;
        tBinding->num_param = 0;
        tBinding->bodydef = FALSE;
        tBinding->label = 0;
        tBinding->localcount=0;
        tBinding->actual_num=0;
        tBinding->param_type=NULL;
        temp->binding = tBinding;

        if(paramlist!=NULL ){
            #ifdef DEBUG
            dbprint(0,"in Function install- temp->binding->num_param is: ",1, INT, ((Funcbcg*)(temp->binding))->num_param);
            #endif
            Funcbcg * extraBind = (Funcbcg*)(temp->binding);
            #ifdef DEBUG
            dbprint(0,"in Function install- extraBind->num_param is: ", 1, INT, extraBind->num_param);
            #endif
			((Funcbcg*)(temp->binding))->num_param = paramlist->listsize;
        }
/*		else
			((Funcb*)(temp->binding))->num_param=0;
*/
		if(((Funcbcg*)(temp->binding))->num_param >0){
			((Funcbcg*)(temp->binding))->param_type = (typecg*)malloc((sizeof(typecg) * paramlist->listsize));
			tempP = paramlist->list;
			for(a=0;a<paramlist->listsize;a++){
				((Funcbcg*)(temp->binding))->param_type[a] = tempP->ttype;
				#ifdef DEBUG
				dbprint(0,"in Function install type is ",1, INT, ((int)tempP->ttype));
				#endif
				if( strcmp("...", tempP->val)==0)
					elip = TRUE;
				else
					elip=FALSE;

				tempP = (listnodeP*) tempP->nextnode;

			}
			if(elip == TRUE) {
				((Funcbcg*)(temp->binding))->actual_num= ((Funcbcg*)(temp->binding))->num_param;
				((Funcbcg*)(temp->binding))->num_param = -1;
			}
		}
/*		else
			((Funcb*)(temp->binding))->param_type = NULL;
 */
//		((Funcb*)(temp->binding))->bodydef = FALSE;
//		((Funcb*)(temp->binding))->label=0;
		return temp;
	}
	else{
		error(0,"name not found\n","");
		return NULL;
	}
}

Entry *createVar(char * name, typecg t_type, int offset){
	Entry * temp;
	temp = (Entry*) malloc(sizeof(Entry));
	temp->name = (char*)strdup(((const char *)name));
	temp->self = VAR;
	//Varb * tBindingV = (Varb *)temp->binding;
    Varbcg * tBindingV = NULL;
	//((Varb*)(temp->binding)) = (Varb*) malloc(sizeof(Varb));
	tBindingV = (Varbcg*) malloc(sizeof(Varbcg));
    tBindingV->type = t_type;

	//((Varb*)(temp->binding))->type = t_type;
	#ifdef DEBUG
	dbprint(0,"in Var install type is ", 1, INT, ((int)t_type));
	#endif
    //((Varb*)(temp->binding))->offset = offset;
	tBindingV->offset = offset;
    temp->binding = tBindingV;
	return temp;
}

Entry *createParam(char * name, typecg t_type, int offset){
	Entry * temp;
	temp = (Entry*) malloc(sizeof(Entry));
	temp->name = (char*)strdup(((const char*)name));
	temp->self = PARAM;
    temp->binding = NULL;
    Parambcg * tBindingP = NULL;
	//((Paramb*)(temp->binding)) = (Paramb*) malloc(sizeof(Varb));
	//((Paramb*)(temp->binding))->type = t_type;
	tBindingP = (Parambcg*) malloc(sizeof(Parambcg));
	tBindingP->type = t_type;
    tBindingP->offset = offset;
    temp->binding = tBindingP;
	#ifdef DEBUG
	dbprint(0,"in Param install type is ",1, INT, ((int)t_type));
	#endif
	//((Paramb*)(temp->binding))->offset = offset;
	return temp;
}

void addtosymtab(Symtab *mysymtab, typecg mytype, List * myList){
	int a;
	listnode * tempN;
	Entry * temp;
	if(mysymtab !=NULL){
		if(myList !=NULL){
			tempN = (listnode*)(myList->list);
			for(a=1;a<=myList->listsize;a++){
				temp = createVar(tempN->val, mytype, offset_counter);
				offset_counter++;
				if((mysymtab->actualStacksize-1) == 0){
					globalcount++;
				}
				install(temp,mysymtab);
				tempN = (listnode*)tempN->nextnode;
			}
			deleteList(myList);
		}
		else error(0,"myList was NULL","");
	}
	else error(0,"mysymtab was NULL","");
}

Entry * lookupB(const char * name, Symtab *symtab){
	Entry ** found;
    found = NULL;
	Entry *temp;
	int a;
	if(name != NULL){
		temp = (Entry*) malloc(sizeof(Entry));
		temp->name = (char *)name;
		for(a=symtab->actualStacksize-1;a>=0;a--){
			found = (Entry**) tfind((void*) temp, (void**) &(symtab->Stack[a]),Ecmp);
			if(found != NULL){
				if(*found !=NULL) break;
			}
		}
		if(found != NULL){
			if(*found ==NULL){
				free(temp->name);
				temp->name = NULL;
				free(temp);
				temp=NULL;
				return NULL;
			}
			else{
				free(temp);
				temp=NULL;
				return *found;
			}
		}
		else{
			free(temp->name);
			temp->name=NULL;
			free(temp);
			temp=NULL;
			return NULL;
		}

	}
    return NULL;
}

boolcg inCscope(const char *name, Symtab *symtab){
        Entry ** found;
        Entry *temp;
        int a;
        if(name != NULL){
                temp = (Entry*) malloc(sizeof(Entry));
                temp->name = (char *)name;
                a=symtab->actualStacksize-1;
                found = (Entry**) tfind((void*) temp, (void**) &(symtab->Stack[a]),Ecmp);
                if(found != NULL){
                        if(*found ==NULL){
                                free(temp->name);
				temp->name=NULL;
                                free(temp);
				temp=NULL;
                                return FALSE;
                        }
                        else{
                                free(temp);
				temp=NULL;
                                return TRUE;
                        }
                }
                else{
                        free(temp->name);
			temp->name=NULL;
                        free(temp);
			temp=NULL;
                        return FALSE;
                }

        }
    return FALSE;
}

int getleveldif(char *name, Symtab *symtab){
	        Entry ** found;
            found = NULL;
	        Entry *temp;
	        int a;
	if(name !=NULL && symtab!=NULL){
	        if(name != NULL){
	                temp = (Entry*) malloc(sizeof(Entry));
	                temp->name = (char *)name;
	                for(a=symtab->actualStacksize-1;a>=0;a--){
	                        found = (Entry**) tfind((void*) temp, (void**) &(symtab->Stack[a]),Ecmp);
	                        if(found != NULL){
	                                if(*found !=NULL) break;
	                        }
	                }
	                if(found != NULL){
	                        if(*found ==NULL){
	                                free(temp->name);
					temp->name=NULL;
	                                free(temp);
					temp=NULL;
	                                return -1;
	                        }
	                        else{
	                                free(temp);
					temp=NULL;
	                                return symtab->actualStacksize-a-1;
//					return symtab->actualStacksize-a;
	                        }
	                }
	                else{
	                        free(temp->name);
				temp->name=NULL;
	                        free(temp);
				temp=NULL;
	                        return -1;
	                }

	        }

	}
	else return -1;
    return -1;
}
