#ifndef _SYMTABCGH
#define _SYMTABCGH
#include "Listcg.h"
#include "typecg.h"
#include <search.h>
#include <stdio.h>
#include "cg.l.h"
#include "main.h"

extern int Line_Number;
extern FILE *infile;
extern int offset_counter;
extern int globalcount;

//extern int warning(char*, char*);

//extern int error(char*,char*);

int Ecmp(const void *Entry1, const void *Entry2);  //comparison function

void printTree(Symtab *symtab);

void Swalk(const void *node, VISIT myorder, int level);

void install(Entry*, Symtab *symtab);

void * lookup(const char * name, Symtab *symtab);
Entry * lookupB(const char * name, Symtab *symtab);
boolcg inCscope(const char *name, Symtab *symtab);

void openscope(Symtab *symtab);

void closescope(Symtab *symtab);

void deleteEntry(Entry * temp);

void deleteTree(Symtab *symtab);

Symtab * createTree(int Stacksize);

Entry* createFunc(char * name, typecg returntype, ListP *paramlist);

Entry* createVar(char * name, typecg t_type, int offset);

Entry* createParam(char * name, typecg t_type, int offset);

void addtosymtab(Symtab* mysymtab, typecg mytype, List * myList);

int getleveldif(char *name, Symtab *mysymtab);

#endif
