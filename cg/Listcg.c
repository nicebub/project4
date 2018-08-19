#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "Listcg.h"
#include "typecg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cg.l.h"
//extern int warning(char*,char*);
//extern int error(char*,char*);

List * mklist(char * inVal){
	List* temp;
	if(inVal != NULL){
		temp = (List*) malloc(sizeof(List));
		temp->list = (listnode*) malloc(sizeof(listnode));
		temp->listsize=1;
		temp->list->val = (char*) strdup(inVal);
		temp->list->nextnode = NULL;
		return temp;
	}
	error("Value isn't correct, could not make List","");
    return NULL;
}

commandList * mkcommandList(char * inVal, ListC* inargs){
	commandList* temp;
	if(inVal != NULL){
		temp = (commandList*) malloc(sizeof(commandList));
		temp->list = (commandlisttype*) malloc(sizeof(commandlisttype));
		temp->listsize=1;
		temp->list->name = (char*) strdup(inVal);
		temp->list->length=1;
		temp->list->paramlist = inargs;
		temp->list->nextcommand = NULL;
		return temp;
	}
	error("Value isn't correct, could not make List","");
    return NULL;
}

ListC * mklistC(char * inVal[2]){
	ListC* temp;
	if(inVal != NULL){
		temp = (ListC*) malloc(sizeof(ListC));
		temp->list = (listnodeC*) malloc(sizeof(listnodeC));
		temp->listsize=1;
		temp->list->argtype[0]=STR;
		temp->list->argtype[1]=STR;
		if(inVal[0]!=NULL)
			temp->list->val[0] = (char*) strdup(inVal[0]);
		else
			temp->list->val[0]=NULL;
		if(inVal[1]!=NULL)
			temp->list->val[1] = (char*) strdup(inVal[1]);
		else
			temp->list->val[1]=NULL;
		temp->list->nextnode = NULL;
		return temp;
	}
	error("Value isn't correct, could not make List","");
    return NULL;
}
ListC * mklistCi(int inVal[2]){
	ListC* temp;
	if(inVal != NULL){
		temp = (ListC*) malloc(sizeof(ListC));
		temp->list = (listnodeC*) malloc(sizeof(listnodeC));
		temp->listsize=1;
		temp->list->argtype[0]=INT;
		temp->list->argtype[1]=INT;
		temp->list->int_val[0] = inVal[0];
		temp->list->int_val[1] = 0;
		temp->list->val[0] = NULL;
		temp->list->val[1] = NULL;
		temp->list->nextnode = NULL;
		return temp;
	}
	error("Value isn't correct, could not make List","");
    return NULL;
}

List * appendList(List * inList, char * inVal){
	//List * tempL;
	listnode * tempN;
	listnode * tempN2;
	//char * tempC;

	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			while(tempN->nextnode != NULL) tempN = (listnode*)tempN->nextnode;
			tempN2 = (listnode*) malloc(sizeof(listnode));
			tempN2->val = (char*) strdup(inVal);
			tempN2->nextnode = NULL;
			//(listnode*)(tempN->nextnode) = (listnode*)tempN2;
            //listnode * tNodeLL = (listnode *) tempN->nextnode;
			listnode * tNodeLL = NULL;
			tNodeLL = (listnode*)tempN2;
            tempN->nextnode = tNodeLL;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}


commandList* appendcommandList(commandList * inList, char * inVal, ListC *inargs){
	//List * tempL;
	commandlisttype* tempN;
	commandlisttype* tempN2;
	//char * tempC;

	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			while(tempN->nextcommand != NULL) tempN = (commandlisttype*)tempN->nextcommand;
			tempN2 = (commandlisttype*) malloc(sizeof(commandlisttype));
			tempN2->name = (char*) strdup(inVal);
			tempN2->length=1;
			tempN2->paramlist=inargs;
			tempN2->nextcommand = NULL;
			//(listnode*)(tempN->nextnode) = (listnode*)tempN2;
            //listnode * tNodeLL = (listnode *) tempN->nextnode;
			commandlisttype * tNodeLL = NULL;
			tNodeLL = (commandlisttype*)tempN2;
            tempN->nextcommand = tNodeLL;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}


ListC * appendListC(ListC * inList, char * inVal, typecg inType){
	//List * tempL;
	listnodeC * tempN;
	listnodeC * tempN2;
	//char * tempC;

	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			tempN->val[1] = (char*) strdup(inVal);
			tempN->argtype[1] = inType;
			//while(tempN->nextnode != NULL) tempN = (listnodeC*)tempN->nextnode;
			//tempN2 = (listnodeC*) malloc(sizeof(listnodeC));
			//tempN2->val[0] = (char*) strdup(inVal[0]);
			//tempN2->val[1] = (char*) strdup(inVal[1]);
			//tempN2->argtype[0] = inType[0];
			//tempN2->argtype[1] = inType[1];
			//tempN2->nextnode = NULL;
			//(listnode*)(tempN->nextnode) = (listnode*)tempN2;
            //listnode * tNodeLL = (listnode *) tempN->nextnode;
//			listnodeC * tNodeLL = NULL;
//			tNodeLL = (listnodeC*)tempN2;
 //           tempN->nextnode = tNodeLL;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}

ListC * appendListCi(ListC * inList, int inVal, typecg inType){
	//List * tempL;
	listnodeC * tempN;
	listnodeC * tempN2;
	//char * tempC;

	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			tempN->int_val[1] = inVal;
			tempN->argtype[1] = inType;
			//while(tempN->nextnode != NULL) tempN = (listnodeC*)tempN->nextnode;
			//tempN2 = (listnodeC*) malloc(sizeof(listnodeC));
			//tempN2->val[0] = (char*) strdup(inVal[0]);
			//tempN2->val[1] = (char*) strdup(inVal[1]);
			//tempN2->argtype[0] = inType[0];
			//tempN2->argtype[1] = inType[1];
			//tempN2->nextnode = NULL;
			//(listnode*)(tempN->nextnode) = (listnode*)tempN2;
            //listnode * tNodeLL = (listnode *) tempN->nextnode;
//			listnodeC * tNodeLL = NULL;
//			tNodeLL = (listnodeC*)tempN2;
 //           tempN->nextnode = tNodeLL;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}


void deleteList(List * inList){
	listnode* temp;
	listnode * temp2;
	if(inList != NULL){
		if(inList->list != NULL){
			temp = inList->list;
			#ifdef DEBUG
			fprintf(stderr,"Deleting List\n");
			#endif
			while(temp->nextnode != NULL){
				#ifdef DEBUG
				fprintf(stderr, "Deleting %s\n", temp->val);
				#endif
				free(temp->val);
				temp->val=NULL;
				temp2 = temp;
				temp = (listnode*)temp->nextnode;
				free(temp2);
				temp2=NULL;
			}
			#ifdef DEBUG
			fprintf(stderr, "Deleting %s\n", temp->val);
			#endif
			free(temp->val);
			temp->val=NULL;
			free(temp);
			temp=NULL;
			free(inList);
			inList=NULL;
		}
	}
}

void deletecommandList(commandList * inList){
	commandlisttype* temp;
	commandlisttype* temp2;
	if(inList != NULL){
		if(inList->list != NULL){
			temp = inList->list;
			#ifdef DEBUG
			fprintf(stderr,"Deleting List\n");
			#endif
			while(temp->nextcommand != NULL){
				#ifdef DEBUG
				fprintf(stderr, "Deleting %s\n", temp->name);
				#endif
				free(temp->name);
				temp->name=NULL;
				deleteListC(temp->paramlist);
				temp->paramlist=NULL;
				temp2 = temp;
				temp = (commandlisttype*)temp->nextcommand;
				free(temp2);
				temp2=NULL;
			}
			#ifdef DEBUG
			fprintf(stderr, "Deleting %s\n", temp->name);
			#endif
			free(temp->name);
			temp->name=NULL;
			deleteListC(temp->paramlist);
			temp->paramlist=NULL;
			free(temp);
			temp=NULL;
			free(inList);
			inList=NULL;
		}
	}
}

void deleteListC(ListC * inList){
	listnodeC* temp;
	listnodeC * temp2;
	if(inList != NULL){
		if(inList->list != NULL){
			temp = inList->list;
			#ifdef DEBUG
			fprintf(stderr,"Deleting List\n");
			#endif
			while(temp->nextnode != NULL){
				#ifdef DEBUG
				fprintf(stderr, "Deleting %s\n", temp->val[0]);
				fprintf(stderr, "Deleting %s\n", temp->val[1]);
				#endif
				free(temp->val[1]);
				temp->val[1]=NULL;
				free(temp->val[0]);
				temp->val[0]=NULL;
				temp2 = temp;
				temp = (listnodeC*)temp->nextnode;
				free(temp2);
				temp2=NULL;
			}
			#ifdef DEBUG
			fprintf(stderr, "Deleting %s\n", temp->val[0]);
			fprintf(stderr, "Deleting %s\n", temp->val[1]);
			#endif
			free(temp->val[1]);
			temp->val[1]=NULL;
			free(temp->val[0]);
			temp->val[0]=NULL;
			free(temp);
			temp=NULL;
			free(inList);
			inList=NULL;
		}
	}
}

void deleteListP(ListP * inList){
	listnodeP* temp;
	listnodeP * temp2;
	if(inList != NULL){
		if(inList->list != NULL){
			temp = inList->list;
			#ifdef DEBUG
			fprintf(stderr,"Deleting List\n");
			#endif
			while(inList->listsize != 0){
				#ifdef DEBUG
				fprintf(stderr, "Deleting %s with type: ", temp->val);

				switch(temp->ttype){
					case(INT): fprintf(stderr, "INT\n"); break;
					case(FLOAT): fprintf(stderr, "FLOAT\n"); break;
					case(CHAR): fprintf(stderr, "CHAR\n"); break;
					case(STR): fprintf(stderr, "STR\n"); break;
					case(REFSTR): fprintf(stderr, "REFSTR\n"); break;
					case(REFINT): fprintf(stderr, "REFINT\n"); break;
					case(REFFLOAT): fprintf(stderr, "REFFLOAT\n"); break;
					case(VOID): fprintf(stderr, "VOID\n"); break;
					default: break;
				}
				#endif
				free(temp->val);
				temp->val=NULL;
				temp2 = temp;
				temp = (listnodeP*)temp->nextnode;
				free(temp2);
				temp2=NULL;
				inList->listsize -= 1;
			}
			free(inList);
			inList=NULL;
		}
	}
}


ListP * appendListP(ListP * inList, char * inVal, typecg inType){
	//ListP * tempL;
	listnodeP * tempN;
    tempN = NULL;
	listnodeP * tempN2;
    tempN2 = NULL;
	//char * tempC;

	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			while(tempN->nextnode != NULL) tempN = (listnodeP*)tempN->nextnode;
			tempN2 = (listnodeP*) malloc(sizeof(listnodeP));
			tempN2->val = (char*) strdup(inVal);
			tempN2->nextnode = NULL;
			tempN2->ttype = inType;
			listnodeP * tNodeLL = NULL;
			//(listnodeP*)(tempN->nextnode) = (listnodeP*) tempN2;
			tNodeLL = (listnodeP*) tempN2;
            tempN->nextnode = tNodeLL;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}

ListP * mklistP(char * inVal, typecg inType){
	ListP* temp;
    temp = NULL;
    if(inVal != NULL){
		temp = (ListP*) malloc(sizeof(ListP));
        temp->list=NULL;
		temp->list = (listnodeP*) malloc(sizeof(listnodeP));
        
		temp->listsize=1;
		temp->list->val = (char*) strdup(inVal);
		temp->list->ttype = inType;
		temp->list->nextnode = NULL;
		return temp;
	}
	else {error("Value isn't correct, could not make List","");
		return NULL;
		}

}

void deleteListE(ListE* inList){
    listnodeE* temp;
    listnodeE * temp2;
	exprtype* exprtemp;
    if(inList != NULL){
        if(inList->list != NULL){
            temp = inList->list;
			#ifdef DEBUG
            fprintf(stderr,"Deleting List\n");
			#endif
            while(inList->listsize != 0){
                exprtemp = temp->expr;
				#ifdef DEBUG
                                fprintf(stderr, "Deleting expr with type: ");
                                switch(exprtemp->type){
                                        case(INT): fprintf(stderr, "INT\n"); break;
                                        case(FLOAT): fprintf(stderr, "FLOAT\n"); break;
                                        case(CHAR): fprintf(stderr, "CHAR\n"); break;
                                        case(STR): fprintf(stderr, "STR\n"); break;
                                        case(REFSTR): fprintf(stderr, "REFSTR\n"); break;
                                        case(REFINT): fprintf(stderr, "REFINT\n"); break;
                                        case(REFFLOAT): fprintf(stderr, "REFFLOAT\n"); break;
                                        case(VOID): fprintf(stderr, "VOID\n"); break;
					default: break;
                                }
				#endif
				free(exprtemp);
				exprtemp=NULL;
                temp2 = temp;
                temp = (listnodeE*)temp->nextnode;
                free(temp2);
                temp2=NULL;
                exprtemp= temp->expr;
                inList->listsize -= 1;
            }
                free(inList);
                inList=NULL;
        }
    }

}

ListE * mklistE(exprtype* inVal){
        ListE* temp;
    temp = NULL;
        if(inVal != NULL){
                temp = (ListE*) malloc(sizeof(ListE));
            temp->list = NULL;
                temp->list = (listnodeE*) malloc(sizeof(listnodeE));
                temp->listsize=1;
		#ifdef DEBUG
		fprintf(stderr, "inVal in mklistE is of type :%d\n", inVal->type);
		#endif
		temp->list->expr = inVal;
		#ifdef DEBUG
		fprintf(stderr," temp in mklistE has type :%d\n",temp->list->expr->type);
		#endif
                temp->list->nextnode = NULL;
                return temp;
        }
        else {error("Value isn't correct, could not make List","");
                return NULL;
                }

}

ListE * appendListE(ListE * inList, exprtype * inexpr){
        //ListE * tempL;
        listnodeE * tempN;
        listnodeE * tempN2;
        //char * tempC;

        if(inList != NULL){
                if(inList->list !=NULL){
                        tempN = inList->list;
                        while(tempN->nextnode != NULL) tempN = (listnodeE*)tempN->nextnode;
                        tempN2 = (listnodeE*) malloc(sizeof(listnodeE));
			tempN2->expr = inexpr;
			#ifdef DEBUG
			fprintf(stderr,"in appendlistE inexpr has type :%d\n tempN2 has type :%d\n",inexpr->type, tempN2->expr->type);
			#endif
                        tempN2->nextnode = NULL;
						listnodeE * tNodeLL = NULL;
                        //(listnodeE*)(tempN->nextnode) = (listnodeE*)tempN2;
                        tNodeLL = (listnodeE*)tempN2;
                    tempN->nextnode = tNodeLL;
                        inList->listsize += 1;
                        return inList;
                }
        }
    return NULL;
}

void printListP(ListP * inList){
    fprintf(stderr,"inList size: %d",inList->listsize);
    if(inList != NULL){
        listnodeP* inNode = inList->list;
        if(inNode !=NULL){
            while(inNode != NULL){
                fprintf(stderr,"\t\t\t---inNode value val: %s\t",inNode->val);
                fprintf(stderr,"inNode ttype: %d---\n",inNode->ttype);
                inNode = (listnodeP*)inNode->nextnode;
                
            }
        }
    }

}
