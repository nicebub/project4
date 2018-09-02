#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "List.h"
#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ucc.l.h"
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


void deleteList(List * inList){
	listnode* temp;
	listnode * temp2;
	if(inList != NULL){
		if(inList->list != NULL){
			temp = inList->list;
			#ifdef DEBUG
			debugprint("Deleting List\n","");
			#endif
			while(temp->nextnode != NULL){
				#ifdef DEBUG
				debugprint( "Deleting from list ", temp->val);
				#endif
				free(temp->val);
				temp2 = temp;
				temp = (listnode*)temp->nextnode;
				free(temp2);
				temp2=NULL;
			}
			#ifdef DEBUG
			debugprint( "Deleting from list ", temp->val);
			#endif
			free(temp->val);
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
			debugprint("Deleting List","");
			#endif
			while(inList->listsize != 0){
				#ifdef DEBUG
				debugprint( "Deleting node with type: ", temp->val);

//				debugprint(temp->val,"");
/*				switch(temp->ttype){
					case(INT): fprintf(stderr, "INT\n"); break;
					case(FLOAT): fprintf(stderr, "FLOAT\n"); break;
					case(CHAR): fprintf(stderr, "CHAR\n"); break;
					case(STR): fprintf(stderr, "STR\n"); break;
					case(REFSTR): fprintf(stderr, "REFSTR\n"); break;
					case(REFINT): fprintf(stderr, "REFINT\n"); break;
					case(REFFLOAT): fprintf(stderr, "REFFLOAT\n"); break;
					case(VOID): fprintf(stderr, "VOID\n"); break;
				}*/
				#endif
				free(temp->val);
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


ListP * appendListP(ListP * inList, char * inVal, type inType){
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

ListP * mklistP(char * inVal, type inType){
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
            debugprint("Deleting List","");
			#endif
            while(inList->listsize != 0){
                exprtemp = temp->expr;
				#ifdef DEBUG
                                debugprint( "Deleting expr with type: ","");
/*                                switch(exprtemp->type){
                                        case(INT): fprintf(stderr, "INT\n"); break;
                                        case(FLOAT): fprintf(stderr, "FLOAT\n"); break;
                                        case(CHAR): fprintf(stderr, "CHAR\n"); break;
                                        case(STR): fprintf(stderr, "STR\n"); break;
                                        case(REFSTR): fprintf(stderr, "REFSTR\n"); break;
                                        case(REFINT): fprintf(stderr, "REFINT\n"); break;
                                        case(REFFLOAT): fprintf(stderr, "REFFLOAT\n"); break;
                                        case(VOID): fprintf(stderr, "VOID\n"); break;
                                }*/
				#endif
				free(exprtemp);
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
		debugprintd( "inVal in mklistE is of type :", inVal->type);
		#endif
		temp->list->expr = inVal;
		#ifdef DEBUG
		debugprintd(" temp in mklistE has type :",temp->list->expr->type);
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
			debugprintd("in appendlistE inexpr has type ",inexpr->type);
			debugprintd("tempN2 has type :", tempN2->expr->type);
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
	#ifdef DEBUG
    debugprintd("inList size: ",inList->listsize);
	#endif
    if(inList != NULL){
        listnodeP* inNode = inList->list;
        if(inNode !=NULL){
            while(inNode != NULL){
				#ifdef DEBUG
                	debugprint("inNode value val: ",inNode->val);
                	debugprintd("inNode ttype: ",inNode->ttype);
                	inNode = (listnodeP*)inNode->nextnode;
				#endif
                
            }
        }
    }

}
