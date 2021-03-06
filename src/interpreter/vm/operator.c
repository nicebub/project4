#include "operator.h"

#define OPERATOR(x,op,y) x op y
#define RELOPERATOR(x, op, y) (x op y == 1)
#define SWITCH_OP(whichtemp,operator,contp1,contp2) switch(operator){\
						case '+':\
							*  whichtemp ## 1 =  OPERATOR((*  whichtemp ## 2), + ,(*  whichtemp ## 1)) ;\
    							break;\
						case '-':\
							*  whichtemp ## 1 =  OPERATOR((*  whichtemp ## 2), - ,( *  whichtemp ## 1)) ;\
							break;\
						case '*':\
							* whichtemp ## 1 =  OPERATOR((* whichtemp ## 2), * ,(* whichtemp ## 1)) ;\
    							break;\
						case '<':\
							* whichtemp ## 1 =  RELOPERATOR((* whichtemp ## 2), < ,(* whichtemp ## 1)) ;\
							break;\
						case '>':\
						  	* whichtemp ## 1 =  !RELOPERATOR((* whichtemp ## 2), > ,(* whichtemp ## 1)) ;\
							break;\
						case '/':\
							if((* whichtemp ## 2) != 0){\
							* whichtemp ## 1 =  OPERATOR((* whichtemp ## 2), / ,(* whichtemp ## 1)) ;\
							break;\
							}\
							error(VMLIBC,"Cannot divide by 0","");\
							break; \
			 }\

    
#define SWITCH_OP2(intype,tempin, used_typnum,operator,contp1,contp2) {\
	void * temp1 = pop_off_stack(&vm_memstack,&used_type ## used_typnum); \
	if(used_type ## used_typnum == INT) \
		*tempin ## 1 =  (int)*(int*)temp1;\
	else \
		*tempin ## 1 =  (float)*(float*)temp1;\
	void * temp2 = pop_off_stack(&vm_memstack,&used_type ## used_typnum); \
	if(used_type ## used_typnum == INT) \
		*tempin ## 2 =  (int)*(int*)temp2;\
	else \
		*tempin ## 2 =  (float)*(float*)temp2;\
    	SWITCH_OP(tempin,operator,contp1,contp2)\
	push(intype,tempin ## 1);\
	return tempin ## 1;\
}

#define SWITCH_RELATE(whichtemp,operator,result)\
						if(strcmp("<=",operator)==0){\
								*result =  RELOPERATOR((*  whichtemp ## 2), <= ,( *  whichtemp ## 1)) ;\
						}\
						else if(strcmp(">=",operator)==0){\
								*result =  RELOPERATOR((*  whichtemp ## 2), >= ,( *  whichtemp ## 1)) ;\
						}\
						else if(strcmp("==",operator)==0){\
								*result =  RELOPERATOR((*  whichtemp ## 2), == ,( *  whichtemp ## 1)) ;\
						}\
						else if(strcmp("!=",operator)==0){\
								*result =  RELOPERATOR((*  whichtemp ## 2), != ,( *  whichtemp ## 1)) ;\
						}\

#define SWITCH_RL2(intype,tempin, used_typnum,operator,result) {\
	void * temp1 = pop_off_stack(&vm_memstack,&used_type ## used_typnum); \
	if(used_type ## used_typnum == INT) \
	*tempin ## 1 =  (int)*(int*)temp1;\
	else \
	*tempin ## 1 =  (float)*(float*)temp1;\
	void * temp2 = pop_off_stack(&vm_memstack,&used_type ## used_typnum); \
	if(used_type ## used_typnum == INT) \
	*tempin ## 2 =  (int)*(int*)temp2;\
	else \
	*tempin ## 2 =  (float)*(float*)temp2;\
    	SWITCH_RELATE(tempin,operator,result)\
	push(intype,result);\
	return result;\
}


void *operate(char operator, typecg intype){
    int *tempin1;
    int *tempin2;
    float *tempfl1;
    float *tempfl2;
    tempin1 = NULL;
    tempin2 = NULL;
    tempfl1 = NULL;
    tempfl2 = NULL;
    
    SWITCH(intype,\
		 REQUESTMEM(tempin1,int, INT)\
		 REQUESTMEM(tempin2,int, INT)\
	   SWITCH_OP2(INT,tempin,5,operator,int,float),\
		 REQUESTMEM(tempfl1,float, FLOAT)\
		 REQUESTMEM(tempfl2,float, FLOAT)
		 SWITCH_OP2(FLOAT,tempfl,5,operator,float,int),return NULL,return NULL)
}
#undef SWITCH_OP
#undef SWITCH_OP2


int * relationship(char *operator, typecg intype){
    int *tempin1;
    int *tempin2;
    float *tempfl1;
    float *tempfl2;
    int *returnval;
    tempin1 = NULL;
    tempin2 = NULL;
    tempfl1 = NULL;
    tempfl2 = NULL;
    returnval = NULL;
    REQUESTMEM(returnval, int, INT)
    *returnval = -1;

    SWITCH(intype,\
		 REQUESTMEM(tempin1,int, INT)\
		 REQUESTMEM(tempin2,int, INT)\
	   SWITCH_RL2(INT,tempin,6,operator,returnval),\
		 REQUESTMEM(tempfl1,float, FLOAT)\
		 REQUESTMEM(tempfl2,float, FLOAT)
		SWITCH_RL2(FLOAT,tempfl,6,operator,returnval),return returnval,return returnval)

}
#undef OPERATOR
#undef SWITCH_RELATE
#undef SWITCH_RL2
