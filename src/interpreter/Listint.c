#if defined(DEBUGON) && defined(LISTDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(LISTDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "debuglib.h"
#include "memlib.h"
#include "Listint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int tag;

struct command_st cmdinf[] = {
    { ALLOC,	 "alloc" },
    { ENTERR,	 "enter" },
    { PUSHS, 	"pushs" },
    { CALL, 	"call" },
    { POPI, 	"popI" },
    { PUSHGA, "pushga" },
    { PUSHA,	 "pusha" },
    { FETCHI, "fetchI" },
    { FETCHR, "fetchR" },
    { STOREI, "storeI" },
    { STORER, "storeR" },
    { PUSHCI, "pushcI" },
    { PUSHCR, "pushcR" },
    { SETRVI, "setrvI" },
    { SETRVR, "setrvR" },
    { RETURNF, "returnf" },
    { RETURN, "return" },
    { LTI,	 "ltI" },
    { LTR,	 "ltR" },
    { LEI,	 "leI" },
    { LER,	 "leR" },
    { JUMP, 	"jump" },
    { JUMPZ, 	"jumpz" },
    { MULI, 	"mulI" },
    { MULR, 	"mulR" },
    { ADDI, 	"addI" },
    { ADDR, 	"addR" }
};


List * mklist(char * inVal){
	List* temp;
    temp = NULL;
	if(inVal != NULL){
		REQUESTMEM(temp, List, GENERIC,*sizeof(List))
    	REQUESTMEM(temp->list, listnode, GENERIC, *sizeof(listnode))
	    
		temp->listsize=1;
		REQUESTMEM(temp->list->val, char, STR,*strlen(inVal)+1)
	    strlcpy(temp->list->val,inVal, strlen(temp->list->val)+1);
		temp->list->nextnode = NULL;
		return temp;
	}
	error(LISTC,"Value isn't correct, could not make List","");
    return NULL;
}
trans_u_list* mkTransList(char* inName, commandList* inList){
	trans_u_list *temp;
	translation_unit* temp_unit;
    temp = NULL;
    temp_unit = NULL;
	if(inName != NULL && inList !=NULL){
		REQUESTMEM(temp, trans_u_list, GENERIC,*sizeof(trans_u_list))
		REQUESTMEM(temp_unit, translation_unit, GENERIC, *sizeof(translation_unit))
		temp->listsize=1;
		temp->list = temp_unit;
	    	REQUESTMEM(temp_unit->name, char, STR, *strlen(inName)+1)
		strlcpy(temp_unit->name,inName,strlen(temp_unit->name)+1);
		temp_unit->commandlist = inList;
		temp_unit->next_trans_unit = NULL;
		return temp;
	}
	error(LISTC,"Could not make Translation Unit List","");
	return NULL;
}

commandList * mkcommandList(char * inVal, ListC* inargs){
	commandList* temp;
#ifdef DEBUG
    char * tempstring;
	tempstring = NULL;
	#endif
    temp = NULL;
	if(inVal != NULL){
		REQUESTMEM(temp, commandList, GENERIC, *sizeof(commandList))
		temp->list = NULL;
		REQUESTMEM(temp->list, commandlisttype, GENERIC, *sizeof(commandlisttype))
	    
		temp->listsize=1;
		temp->list->name = NULL;
		REQUESTMEM(temp->list->name, char, STR, *strlen(inVal)+1)
	    strlcpy(temp->list->name, inVal, strlen(temp->list->name)+1);
	    FORCMD(temp->list->name,cmdinf,temp->list->name_enm)
		#ifdef DEBUG
	    for(int a = 0; a<sizeof(command_name_strings);a++){
		   if(strcmp(temp->list->name, command_name_strings[a]) == 0){
			  tempstring = (char*) command_name_strings[a];
			  dbprint(LISTC,"found something: ", 1,STR,tempstring);
			  break;
		   }
	    }
		#endif

		if(inargs == NULL){
			temp->list->length=0;
		}
		else
			temp->list->length = inargs->listsize;
		temp->list->paramlist = inargs;
		temp->list->nextcommand = NULL;
		#ifdef DEBUG
		debugprintd(LISTC,"Checking CommandList size ",temp->listsize);
		debugprintc(LISTC,"Checking CommandList Accuracy ",temp);
		#endif
		return temp;
	}
	error(LISTC,"Value isn't correct, could not make List","");
    return NULL;
}

ListC * mklistC(char * inVal[2]){
	ListC* temp;
    temp = NULL;
	if(inVal != NULL){
		REQUESTMEM(temp, ListC, GENERIC, *sizeof(ListC))
	    temp->list = NULL;
		REQUESTMEM(temp->list, listnodeC, GENERIC, *sizeof(listnodeC))
		temp->listsize=1;
		temp->list->argtype[0]=STR;
		temp->list->argtype[1]=STR;
	    temp->list->val[0] = NULL;
	    temp->list->val[1] = NULL;
	    if(inVal[0]!=NULL){

		   REQUESTMEM(temp->list->val[0], char, STR,*strlen(inVal[0])+1)
		   strlcpy(temp->list->val[0],inVal[0], strlen(temp->list->val[0])+1);
	    }
		else
			temp->list->val[0]=NULL;
	    if(inVal[1]!=NULL){
 		   REQUESTMEM(temp->list->val[1], char, STR,*strlen(inVal[1])+1)
		   strlcpy(temp->list->val[1],inVal[1], strlen(temp->list->val[1])+1);
	    }
	   else
			temp->list->val[1]=NULL;
		temp->list->nextnode = NULL;
		return temp;
	}
	error(LISTC,"Value isn't correct, could not make List","");
    return NULL;
}

ListC * mklistCi(int inVal[2]){
	ListC* temp;
    temp = NULL;
	if(inVal != NULL){
		REQUESTMEM(temp,ListC, GENERIC, *sizeof(ListC))
	   REQUESTMEM(temp->list, listnodeC, GENERIC, *sizeof(listnodeC))
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
	error(LISTC,"Value isn't correct, could not make List","");
    return NULL;
}

List * appendList(List * inList, char * inVal){
	listnode * tempN;
	listnode * tempN2;
    tempN = NULL;
    tempN2 = NULL;
	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			while(tempN->nextnode != NULL) tempN = (listnode*)tempN->nextnode;
			REQUESTMEM(tempN2, listnode, GENERIC, *sizeof(listnode))
			REQUESTMEM(tempN2->val, char, STR,*strlen(inVal)+1)
		    strlcpy(tempN2->val,inVal, strlen(tempN2->val)+1);

			tempN2->nextnode = NULL;
			listnode * tNodeLL = NULL;
			tNodeLL = (listnode*)tempN2;
            tempN->nextnode = tNodeLL;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}
trans_u_list * prependTransList(trans_u_list* inTransList, char* inName, commandList* inList){
	translation_unit *temp;
	translation_unit *temp2;
	temp = NULL;
	temp2 = NULL;
	if(inTransList != NULL){
		if(inTransList->list != NULL){
			#ifdef DEBUG
			debugprint(LISTC,"Appending Translation Unit to Trans_List","");
			#endif
			REQUESTMEM(temp2, translation_unit, GENERIC, *sizeof(translation_unit))
	 		REQUESTMEM(temp2->name, char, STR,*strlen(inName)+1)
		    strlcpy(temp2->name,inName, strlen(temp2->name)+1);
			temp2->commandlist = inList;
			temp2->next_trans_unit = inTransList->list;
			inTransList->list = temp2;
			inTransList->listsize +=1;
				return inTransList;
		}
		return NULL;
	}
	return NULL;
}
trans_u_list * appendTransList(trans_u_list* inTransList, char* inName, commandList* inList){
	translation_unit *temp;
	translation_unit *temp2;
	temp = NULL;
	temp2 = NULL;
	if(inTransList != NULL){
		if(inTransList->list != NULL){
			#ifdef DEBUG
			debugprint(LISTC,"Appending Translation Unit to Trans_List","");
			#endif
			temp = inTransList->list;
			while(temp != NULL && temp->next_trans_unit != NULL) temp = temp->next_trans_unit;
			REQUESTMEM(temp2, translation_unit, GENERIC, *sizeof(translation_unit))
	 		REQUESTMEM(temp2->name, char, STR,*strlen(inName)+1)
		    strlcpy(temp2->name,inName, strlen(temp2->name)+1);

			temp2->commandlist = inList;
			temp2->next_trans_unit = NULL;
			temp->next_trans_unit = temp2;
			inTransList->listsize +=1;
				return inTransList;
		}
		return NULL;
	}
	return NULL;
}

commandList* appendcommandList(commandList * inList, char * inVal, ListC *inargs){
	commandlisttype* tempN;
	commandlisttype* tempN2;
    tempN = NULL;
    tempN2 = NULL;
	if(inList != NULL){
		if(inList->list !=NULL){
			#ifdef DEBUG
			debugprint(LISTC,"Appending Commands to the command list", inVal);
			#endif
			tempN = inList->list;
			while(tempN != NULL && tempN->nextcommand != NULL) tempN = (commandlisttype*)tempN->nextcommand;
			REQUESTMEM(tempN2,commandlisttype,GENERIC, *sizeof(commandlisttype))
				
			REQUESTMEM(tempN2->name, char, STR, *strlen(inVal)+1)
		    strlcpy(tempN2->name,inVal,strlen(tempN2->name)+1);
		    FORCMD(tempN2->name,cmdinf,tempN2->name_enm)

		    
		    if(inargs!=NULL){
				tempN2->length=inargs->listsize;
			}
			else{
				tempN2->length=0;
			}
			tempN2->paramlist=inargs;
			tempN2->nextcommand = NULL;
			commandlisttype * tNodeLL = NULL;
			tNodeLL = (commandlisttype*)tempN2;
            tempN->nextcommand = tNodeLL;
			inList->listsize += 1;
			#ifdef DEBUG
			debugprintd(LISTC,"Checking CommandList size ",inList->listsize);
			debugprintc(LISTC,"Checking CommandList Accuracy ",inList);
			#endif
			return inList;
		}
	}
    return NULL;
}


ListC * appendListC(ListC * inList, char * inVal, typecg inType){
	listnodeC * tempN;
    tempN = NULL;
	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
	 		REQUESTMEM(tempN->val[1], char, STR,*strlen(inVal)+1)
		    strlcpy(tempN->val[1],inVal, strlen(tempN->val[1])+1);

			tempN->argtype[1] = inType;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}

ListC * appendListCi(ListC * inList, int inVal, typecg inType){
	listnodeC * tempN;
    tempN= NULL;
	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			tempN->int_val[1] = inVal;
			tempN->argtype[1] = inType;
			inList->listsize += 1;
			return inList;
		}
	}
    return NULL;
}

translation_unit * getlastUnit(trans_u_list* inTransList){
	translation_unit * temp;
	temp = NULL;
	if(inTransList !=NULL){
		temp = inTransList->list;
		while(temp !=NULL && temp->next_trans_unit !=NULL){
			temp = temp->next_trans_unit;
		}
		if(temp !=NULL){
			return temp;
		}
		return NULL;
	}
	return NULL;
}



void deleteList(List * inList){
	listnode* temp;
	listnode * temp2;
    temp = NULL;
    temp2 = NULL;
	if(inList != NULL){
		if(inList->list != NULL){
			temp = inList->list;
			#ifdef DEBUG
			dbprint(LISTC,"Deleting List",0);
			#endif
			while(temp->nextnode != NULL){
				#ifdef DEBUG
				dbprint(LISTC, "Deleting ", 1, STR, (void*) temp->val);
				#endif
				free(temp->val);
				temp->val=NULL;
				temp2 = temp;
				temp = (listnode*)temp->nextnode;
				free(temp2);
				temp2=NULL;
			}
			#ifdef DEBUG
			dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->val);
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
			dbprint(LISTC,"Deleting List", 0);
			#endif
			while(temp->nextcommand != NULL){
				#ifdef DEBUG
				dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->name);
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
			dbprint(LISTC, "Deleting ",1,  STR,(void*) temp->name);
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


void deleteTransList(trans_u_list* inList){
	translation_unit* temp;
	translation_unit* temp2;
	if(inList != NULL){
		if(inList->list != NULL){
			temp = inList->list;
			#ifdef DEBUG
			dbprint(LISTC,"Deleting Translation Unit List", 0);
			#endif
			while(temp->next_trans_unit != NULL){
				#ifdef DEBUG
				dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->name);
				#endif
				free(temp->name);
				temp->name=NULL;
				deletecommandList(temp->commandlist);
				temp->commandlist=NULL;
				temp2 = temp;
				temp = (translation_unit*)temp->next_trans_unit;
				free(temp2);
				temp2=NULL;
			}
			#ifdef DEBUG
			dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->name);
			#endif
			free(temp->name);
			temp->name=NULL;
			deletecommandList(temp->commandlist);
			temp->commandlist=NULL;
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
			dbprint(LISTC,"Deleting List", 0);
			#endif
			while(temp->nextnode != NULL){
				#ifdef DEBUG
				dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->val[0]);
				dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->val[1]);
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
			dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->val[0]);
			dbprint(LISTC, "Deleting ", 1, STR,(void*) temp->val[1]);
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
			dbprint(LISTC,"Deleting List", 0);
			#endif
			while(inList->listsize != 0){
				#ifdef DEBUG
				dbprint(LISTC, "Deleting value with name: ", 1, STR,(void*) temp->val);
				dbprint(LISTC, "and type: ", 1, STR,(void*) typecg_strings[temp->ttype]);
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

trans_u_list * concat_trans_unit_list(trans_u_list* front, trans_u_list* back){
	translation_unit * temp;
	temp = NULL;
	if(front != NULL){
		if(back != NULL){
			temp = getlastUnit(front);
			if(temp !=NULL){
				temp->next_trans_unit = back->list;
				front->listsize += back->listsize;
			}
		}
		return front;
	}
	return NULL;
}


ListP * appendListP(ListP * inList, char * inVal, typecg inType){
	listnodeP * tempN;
    tempN = NULL;
	listnodeP * tempN2;
    tempN2 = NULL;

	if(inList != NULL){
		if(inList->list !=NULL){
			tempN = inList->list;
			while(tempN->nextnode != NULL) tempN = (listnodeP*)tempN->nextnode;
			REQUESTMEM(tempN2, listnodeP, GENERIC, *sizeof(listnodeP))
			REQUESTMEM(tempN2->val, char, STR, *strlen(inVal)+1)

		    strlcpy(tempN2->val,inVal, strlen(tempN2->val)+1);

			tempN2->nextnode = NULL;
			tempN2->ttype = inType;
			listnodeP * tNodeLL = NULL;
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
		REQUESTMEM(temp, ListP, GENERIC, *sizeof(ListP))
        temp->list=NULL;
		REQUESTMEM(temp->list, listnodeP, GENERIC,*sizeof(listnodeP))
	   
		temp->listsize=1;
		REQUESTMEM(temp->list->val, char, STR, *strlen(inVal)+1)
	   strlcpy(temp->list->val,inVal, strlen(temp->list->val)+1);

		temp->list->ttype = inType;
		temp->list->nextnode = NULL;
		return temp;
	}
	else {error(LISTC,"Value isn't correct, could not make List","");
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
            dbprint(LISTC,"Deleting List", 0);
			#endif
		  exprtemp = temp->expr;
            while(exprtemp != NULL && inList->listsize != 0){
                exprtemp = temp->expr;
				#ifdef DEBUG
                                dbprint(LISTC, "Deleting expr with type: ", 1, STR,(void*) typecg_strings[exprtemp->type]);
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
			REQUESTMEM(temp, ListE, GENERIC, *sizeof(ListE))
            temp->list = NULL;
			REQUESTMEM(temp->list, listnodeE, GENERIC, *sizeof(listnodeE))
                temp->listsize=1;
		#ifdef DEBUG
		dbprint(LISTC, "inVal in mklistE is of type: ", 1, STR,(void*) typecg_strings[inVal->type]);
		#endif
		temp->list->expr = inVal;
		#ifdef DEBUG
		dbprint(LISTC," temp in mklistE has type: ", 1, STR,(void*) typecg_strings[temp->list->expr->type]);
		#endif
                temp->list->nextnode = NULL;
                return temp;
        }
        else {error(LISTC,"Value isn't correct, could not make List","");
                return NULL;
                }

}

ListE * appendListE(ListE * inList, exprtype * inexpr){
        listnodeE * tempN;
        listnodeE * tempN2;
    tempN = NULL;
    tempN2 = NULL;

        if(inList != NULL){
                if(inList->list !=NULL){
                        tempN = inList->list;
                        while(tempN->nextnode != NULL) tempN = (listnodeE*)tempN->nextnode;
						REQUESTMEM(tempN2, listnodeE, GENERIC, *sizeof(listnodeE))
			tempN2->expr = inexpr;
			#ifdef DEBUG
			dbprint(LISTC,"in appendlistE inexpr has type: ", 1, STR,(void*) typecg_strings[inexpr->type]);
			dbprint(LISTC, "tempN2 has type: ", 1, STR ,(void*) typecg_strings[tempN2->expr->type]);
			#endif
                        tempN2->nextnode = NULL;
						listnodeE * tNodeLL = NULL;
                        tNodeLL = (listnodeE*)tempN2;
                    tempN->nextnode = tNodeLL;
                        inList->listsize += 1;
                        return inList;
                }
        }
    return NULL;
}

void printListP(ListP * inList){
    dbprint(LISTC,"inList size: ", 1, INT ,inList->listsize);
    if(inList != NULL){
        listnodeP* inNode = inList->list;
        if(inNode !=NULL){
            while(inNode != NULL){
                dbprint(LISTC,"\t\t\t---inNode value val: ", 1, STR,(void*) inNode->val);
                dbprint(LISTC,"inNode ttype: ", 1, STR,(void*) typecg_strings[inNode->ttype]);
                inNode = (listnodeP*)inNode->nextnode;
                
            }
        }
    }

}

void printListC(ListC * inList){
	if(inList != NULL){
		if(inList->list != NULL){
			if(inList->list->argtype[0] == STR){
				if((&inList->list->val) != NULL){
					debugprint(LISTC,"val[0]", inList->list->val[0]);
				}
			}
			else{
				debugprintd(LISTC,"int_val[0]", inList->list->int_val[0]);
				
			}
			if(inList->list->val[1] != NULL && inList->list->argtype[1] == STR){
				debugprint(LISTC,"val[1]", inList->list->val[1]);
			}
			else if(inList->list->argtype[1] != STR && inList->listsize > 1){
				debugprintd(LISTC,"int_val[1]", inList->list->int_val[1]);
			}			
		}
		else
			debugprint(LISTC,"List is empty","");
	}
	else
		debugprint(LISTC,"List is empty","");
}

void printcommandList(commandList * inList){
	if(inList != NULL){
		if(inList->list != NULL){
			if(inList->list->name != NULL){
				
				debugprint(LISTC,"First command of command list", inList->list->name);
				
			}
			else{
				debugprint(LISTC,"Command List Empty","");
				
			}
			
			debugprintd(LISTC,"Command List listsize", inList->listsize);
			commandlisttype* temp = inList->list;
			for(int i = 0; temp !=NULL && i<inList->listsize; i++){
				debugprint(LISTC,"Command Name:",temp->name);
				debugprintd(LISTC,"Command Number of Arguments",temp->length);
				printListC(temp->paramlist);
				temp = temp->nextcommand;
			}
		}
		else
			debugprint(LISTC,"List is empty","");
	}
}

void printTransList(trans_u_list *inList){
	if(inList !=NULL){
		if(inList->list !=NULL){
			debugprintd(LISTC,"Translation Units Available", inList->listsize);
			translation_unit *temp = inList->list;
			for(int i = 0; temp !=NULL && i<inList->listsize; i++){
				debugprint(LISTC,"Translation Unit Name:",temp->name);
				printcommandList(temp->commandlist);
				temp = temp->next_trans_unit;
			}
			
		}
		else{
			debugprint(LISTC,"Translation Unit List Empty","");
		}
	}
	else{
		debugprint(LISTC,"Translation Unit List Empty","");		
	}
}
