#ifndef STORE_H
#define STORE_H


#include "../typeint.h"
#include "../useful.def"
#include "../memlib.h"
#include "pop.h"

#include "memory.h"

#include <stdlib.h>
#include <stdio.h>

extern typecg used_type5, used_type6;


void store(void* value, typecg intype, int inscope,int *offset);
void in_loop_store(typecg intype);

#endif
