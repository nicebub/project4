#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "symtab.h"
#include "type.h"
#include "List.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include "ucc.l.h"
#include <string.h>

//extern int error(char*,char*);

void openscope(Symtab *symtab){
	if(symtab->actualStacksize == symtab->Stacksize)
		error("Scope Stack already too full","");
	else{
		#ifdef DEBUG
		debugprint("Opening new Scope","");
		#endif
	 	symtab->actualStacksize += 1;

		offset_counter=5;
	}
}

void closescope(Symtab *symtab){
	Entry * temp;
	if(symtab->actualStacksize == 1)
		error("Cannot close Global scope","");
	else{
		#ifdef DEBUG
		debugprint("Closing Scope","");
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
		error("cannot lookup variable without a name","");
		return NULL;
	}
}

void install(Entry * temp,Symtab *symtab){
	Entry ** found;
	found = (Entry**) tsearch((void*) temp, (void**) &((symtab->Stack[symtab->actualStacksize-1])),Ecmp);

	if(*found !=temp)
		error("symbol already declared in current scope","");
}

int Ecmp(const void *Entry1, const void *Entry2){
	return strcmp( ((Entry*)Entry1)->name, ((Entry*)Entry2)->name);
}

#ifdef DEBUG
void printTree(Symtab *symtab){
	if(symtab->Stack[0] != NULL)
		twalk((void*) (symtab->Stack[symtab->actualStacksize-1]), Swalk);
}

void Swalk(const void *node, VISIT myorder, int level){
		Entry * temp;
		Funcb * tempb;
		int a=0;
	if(myorder == preorder || myorder == leaf){
		temp = (*(Entry**) node);
		tempb = ((Funcb*)(temp->binding));
		switch(temp->self){

		case(FUNC):		debugprint("Name: ", temp->name);
						debugprint("Return Type: ","");
					switch(tempb->returntype){
						case(INT):	debugprint("INT",""); break;
						case(FLOAT):	debugprint("FLOAT",""); break;
						case(VOID):	debugprint("VOID",""); break;
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
                    }
					debugprintd("Body Defined: ", tempb->bodydef);
					debugprintd("Number Parameters: ",tempb->num_param);
					if(tempb->num_param >0){
						debugprint("Parameter Types: ","");
						for(a=0;a<tempb->num_param;a++){
							switch(tempb->param_type[a]){
								case(INT):	fprintf(stderr,"INT ");break;
								case(FLOAT):	fprintf(stderr,"FLOAT "); break;
								case(REFINT):	fprintf(stderr,"REFINT "); break;
								case(REFFLOAT):	fprintf(stderr, "REFFLOAT "); break;
								case(REFSTR):	fprintf(stderr, "REFSTR "); break;
								case(STR):	fprintf(stderr, "STR "); break;
								case(VOID):	fprintf(stderr, "VOID "); break;
                                case CHAR:
                                    
                                    break;
                            }
						}
					}
					if(tempb->num_param == -1){
						debugprint("Parameter Types: ","");
                        for(a=0;a<tempb->actual_num;a++){
                            switch(tempb->param_type[a]){
                                                                case(INT):      fprintf(stderr,"INT ");break;
                                                                case(FLOAT):    fprintf(stderr,"FLOAT "); break;
                                                                case(REFINT):   fprintf(stderr,"REFINT "); break;
                                                                case(REFFLOAT): fprintf(stderr, "REFFLOAT "); break;
                                                                case(REFSTR):   fprintf(stderr, "REFSTR "); break;
                                                                case(STR):      fprintf(stderr, "STR "); break;
                                                                case(VOID):     fprintf(stderr, "VOID "); break;
                                case CHAR:
                                
                                break;
                            }
                        }
						//fprintf(stderr,"...");

					}
					debugprint("\n\n","");

					break;
		case(PARAM):		debugprint("Name: ", temp->name);
							debugprint("Type: ","");
					switch( ((Paramb*)(temp->binding))->type){
						case(INT):	debugprint("INT",""); break;
						case(FLOAT):	debugprint("FLOAT",""); break;
                        case VOID:
                            
                            break;
                        case CHAR:
                            
                            break;
                        case(STR):	fprintf(stderr, "STR\n"); break;
						case(REFINT):	fprintf(stderr, "REFINT\n"); break;
						case(REFFLOAT):	fprintf(stderr, "REFFLOAT\n"); break;
						case(REFSTR):	fprintf(stderr, "REFSTR\n"); break;
					}
					fprintf(stderr,"Offset: %d\n\n", ((Paramb*)(temp->binding))->offset);

					break;
		case(VAR):		debugprint("Name: ", temp->name);
						debugprint("Type: ","");
					switch(((Varb*)(temp->binding))->type){
						case(INT):	fprintf(stderr, "INT\n");
								break;
						case(FLOAT):	fprintf(stderr, "FLOAT\n");

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
                    }
					debugprintd("Offset: ", ((Varb*)(temp->binding))->offset);

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
	for(a=0;a<temp->Stacksize;a++) temp->Stack[a]=NULL;
	return temp;
}

void deleteTree(Symtab *symtab){
	Entry * temp;
	//Entry ** found;
	if(symtab != NULL){
	   #ifdef DEBUG
	   debugprint("Deleting Tree: ","");
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
	    debugprint("Deleting Stack","");
	    #endif
	    free(symtab->Stack);
	    #ifdef DEBUG
	    debugprint("Deleting Symbol Table","");
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
						debugprint("Deleting Function ", temp->name);
						#endif
						if( ((Funcb*)(temp->binding))->num_param > 0 || ((Funcb*)(temp->binding))->num_param == -1)
							free( ((Funcb*)(temp->binding))->param_type);
						free(temp->binding);
						free(temp->name);
						free(temp);
						temp=NULL;
						break;
			case(VAR):
						#ifdef DEBUG
						debugprint("Deleting variable ", temp->name);
						#endif
						free(temp->name);
						free(temp->binding);
						free(temp);
						temp=NULL;
						break;
			case(PARAM):
						#ifdef DEBUG
						debugprint("Deleteing Parameter ", temp->name);
						#endif
						free(temp->name);
						free(temp->binding);
						free(temp);
						temp= NULL;
						break;
			default:	
						#ifdef DEBUG	
						debugprint("Error in Node, doesn't have correct binding","");
						#endif
						;
		}
	}

}


Entry *createFunc(char * name, type returntype, ListP* paramlist){
	Entry * temp;
	listnodeP * tempP;
	int a;
	bool elip=FALSE;
	temp = (Entry*) malloc(sizeof(Entry));
	if(name !=NULL){
		temp->name = (char*)strdup(((const char*)name));
		temp->self = FUNC;
		//Funcb * tBinding = (Funcb*)temp->binding;
        Funcb * tBinding = NULL;
		//((Funcb*)(temp->binding)) = (Funcb*) malloc(sizeof(Funcb));
		//((Funcb*)(temp->binding))->returntype = returntype;
		tBinding = (Funcb*) malloc(sizeof(Funcb));
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
            debugprintd("in Function install- temp->binding->num_param is: ", ((Funcb*)(temp->binding))->num_param);
            #endif
            Funcb * extraBind = (Funcb*)(temp->binding);
            #ifdef DEBUG
            debugprintd("in Function install- extraBind->num_param is: ", extraBind->num_param);
            #endif
			((Funcb*)(temp->binding))->num_param = paramlist->listsize;
        }
/*		else
			((Funcb*)(temp->binding))->num_param=0;
*/
		if(((Funcb*)(temp->binding))->num_param >0){
			((Funcb*)(temp->binding))->param_type = (type*)malloc((sizeof(type) * paramlist->listsize));
			tempP = paramlist->list;
			for(a=0;a<paramlist->listsize;a++){
				((Funcb*)(temp->binding))->param_type[a] = tempP->ttype;
				#ifdef DEBUG
				fprintf(stderr,"in Function install type is %d\n",tempP->ttype);
				#endif
				if( strcmp("...", tempP->val)==0)
					elip = TRUE;
				else
					elip=FALSE;

				tempP = (listnodeP*) tempP->nextnode;

			}
			if(elip == TRUE) {
				((Funcb*)(temp->binding))->actual_num= ((Funcb*)(temp->binding))->num_param;
				((Funcb*)(temp->binding))->num_param = -1;
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
		error("name not found\n","");
		return NULL;
	}
}

Entry *createVar(char * name, type t_type, int offset){
	Entry * temp;
	temp = (Entry*) malloc(sizeof(Entry));
	temp->name = (char*)strdup(((const char *)name));
	temp->self = VAR;
	//Varb * tBindingV = (Varb *)temp->binding;
    Varb * tBindingV = NULL;
	//((Varb*)(temp->binding)) = (Varb*) malloc(sizeof(Varb));
	tBindingV = (Varb*) malloc(sizeof(Varb));
    tBindingV->type = t_type;

	//((Varb*)(temp->binding))->type = t_type;
	#ifdef DEBUG
	fprintf(stderr,"in Var install type is :%d\n",t_type);
	#endif
    //((Varb*)(temp->binding))->offset = offset;
	tBindingV->offset = offset;
    temp->binding = tBindingV;
	return temp;
}

Entry *createParam(char * name, type t_type, int offset){
	Entry * temp;
	temp = (Entry*) malloc(sizeof(Entry));
	temp->name = (char*)strdup(((const char*)name));
	temp->self = PARAM;
    temp->binding = NULL;
    Paramb * tBindingP = NULL;
	//((Paramb*)(temp->binding)) = (Paramb*) malloc(sizeof(Varb));
	//((Paramb*)(temp->binding))->type = t_type;
	tBindingP = (Paramb*) malloc(sizeof(Paramb));
	tBindingP->type = t_type;
    tBindingP->offset = offset;
    temp->binding = tBindingP;
	#ifdef DEBUG
	fprintf(stderr,"in Param install type is :%d\n",t_type);
	#endif
	//((Paramb*)(temp->binding))->offset = offset;
	return temp;
}

void addtosymtab(Symtab *mysymtab, type mytype, List * myList){
	int a;
	listnode * tempN;
	Entry * temp;
	if(mysymtab !=NULL){
		if(myList !=NULL){
			tempN = (listnode*)(myList->list);
			for(a=1;a<=myList->listsize;a++){
				temp= createVar(tempN->val, mytype, offset_counter);
				offset_counter++;
				if((mysymtab->actualStacksize-1) == 0){
					globalcount++;
				}
				install(temp,mysymtab);
				tempN = (listnode*)tempN->nextnode;
			}
			deleteList(myList);
		}
		else error("myList was NULL","");
	}
	else error("mysymtab was NULL","");
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
			if(symtab->Stack[0] != NULL)
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
				return *found;
			}
		}
		else{
			free(temp->name);
			free(temp);
			return NULL;
		}

	}
    return NULL;
}

bool inCscope(const char *name, Symtab *symtab){
        Entry ** found;
	   found = NULL;
        Entry *temp;
        int a;
        if(name != NULL){
                temp = (Entry*) malloc(sizeof(Entry));
                temp->name = (char *)name;
                a=symtab->actualStacksize-1;
			 if(symtab->Stack[0] != NULL)
                	found = (Entry**) tfind((void*) temp, (void**) &(symtab->Stack[a]),Ecmp);
                if(found != NULL){
                        if(*found ==NULL){
                                free(temp->name);
                                free(temp);
                                return FALSE;
                        }
                        else{
                                free(temp);
                                return TRUE;
                        }
                }
                else{
                        free(temp->name);
                        free(temp);
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
					 	if(symtab->Stack[0] != NULL)
	                        		found = (Entry**) tfind((void*) temp, (void**) &(symtab->Stack[a]),Ecmp);
	                        if(found != NULL){
	                                if(*found !=NULL) break;
	                        }
	                }
	                if(found != NULL){
	                        if(*found ==NULL){
	                                free(temp->name);
	                                free(temp);
	                                return -1;
	                        }
	                        else{
	                                free(temp);
	                                return symtab->actualStacksize-a-1;
//					return symtab->actualStacksize-a;
	                        }
	                }
	                else{
	                        free(temp->name);
	                        free(temp);
	                        return -1;
	                }

	        }

	}
	else return -1;
    return -1;
}
