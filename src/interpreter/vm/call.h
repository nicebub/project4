#ifndef CALL_H
#define CALL_H

#ifndef MAX_STR_CONST
#define MAX_STR_CONST 50
#endif

#ifndef MAX_FUNCTIONS
#define MAX_FUNCTIONS 30
#endif


#include <stdio.h>
#include <string.h>
#include "../memlib.h"
#include "stacklib.h"
#include "store.h"
#include "printf.h"


extern typecg used_type5, used_type6;

void call(char * inname, int count, ...);
void call_other(char * inname, int count, ...);
int in_loop_call(commandlisttype** currentcommand, translation_unit** allotherunits, \
	   int *commandnum, translation_unit** current_unit, int *c);

#endif
