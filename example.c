#ifdef DEBUGON
#define DEBUG
//int yydebug = 1;
#endif
#include "data.h"
#define YYSTYPE data
#include "ucc.tab.h"
#include "List.h"
#include "command.h"
#include "main.h"
#include "trans.h"
#include "ucc.l.h"

#include "symtab.h"
#include <search.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"


int main(int argc, char** argv){
	Symtab *symtab;
	Entry * temp=NULL;
	//Entry ** found=NULL;
	//int a;
	type * types=NULL;
	void * binding=NULL;
	ListP * tempPList = NULL;
	symtab = createTree(100);
	temp = createFunc("function1", INT, NULL);
	install(temp,symtab);
	types = (type*) malloc(sizeof(type) *1);
	types[0] = FLOAT;
	tempPList = mklistP("2", FLOAT);
	temp = createFunc("function2", FLOAT, tempPList);
	install(temp,symtab);
	temp = createVar("Var1", INT, 01234);
	install(temp,symtab);
	temp = createVar("apple", FLOAT, 13441);
	install(temp,symtab);
	temp= createParam("beta", INT, 12353);
	install(temp,symtab);
	temp = createParam("beta_3", FLOAT, 1323242);
	install(temp, symtab);
	temp = createVar("beta3", FLOAT, 134234);
	install(temp, symtab);
	printTree(symtab);
	binding = lookup("apple",symtab);
	if(binding == NULL) fprintf(stderr, "Does not exist\n");
	else{
		fprintf(stderr, "Name: %s\nType: ", "apple");
		switch( ((Varb*)temp->binding)->type){
			case( INT):	fprintf(stderr, "INT\n"); break;
			case(FLOAT):	fprintf(stderr, "FLOAT\n"); break;
		}
		fprintf(stderr, "Offset: %d\n\n", ((Varb*)temp->binding)->offset);
	}
	openscope(symtab);
	types = (type*) malloc(sizeof(type) * 3);
	types[0]=INT;
	types[1]=FLOAT;
	types[2] = STR;
	deleteListP(tempPList);
	tempPList = NULL;
	tempPList = mklistP("420", INT);
	tempPList = appendListP(tempPList, "3.14159", FLOAT);
	tempPList = appendListP(tempPList, "Four Score and Seven Years Ago!\n", STR);
	temp = createFunc("Newfunc", INT, tempPList);
	install(temp, symtab);
	printTree(symtab);
	types = (type*) malloc(sizeof(type)*7);
	types[0]=INT;
	types[1] = FLOAT;
	types[2]=STR;
	types[3]=REFINT;
	types[4]=REFSTR;
	types[5] = STR;
	types[6]=FLOAT;
	deleteListP(tempPList);
	tempPList = NULL;
	tempPList = mklistP("1", INT);
	tempPList = appendListP(tempPList, "4.22", FLOAT);
	tempPList = appendListP(tempPList, "STRing!", STR);
	tempPList = appendListP(tempPList, "45", REFINT);
	tempPList = appendListP(tempPList, "REFSTRRRRR", REFSTR);
	tempPList = appendListP(tempPList, "ANOTHER_STRING", STR);
	tempPList = appendListP(tempPList, "0.78", FLOAT);
	temp = createFunc("open_me", VOID, tempPList);
	install(temp, symtab);
	printTree(symtab);
	openscope(symtab);
	printTree(symtab);
	closescope(symtab);
	closescope(symtab);
	openscope(symtab);
	temp = createParam("Parameter_1", STR, 234324);
	install(temp, symtab);
	printTree(symtab);
	openscope(symtab);
	temp = createParam("Parameter_2", STR, 32435324);
	install(temp, symtab);
	printTree(symtab);
	openscope(symtab);
	temp = createParam("Parameter_3", INT, 324234234);
	install(temp, symtab);
	printTree(symtab);
	openscope(symtab);



	printTree(symtab);


	deleteTree(symtab);
	return 0;
}
