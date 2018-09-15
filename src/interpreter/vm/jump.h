#ifndef JUMP_H
#define JUMP_H

#include <stdlib.h>

#include "pop.h"
#include "store.h"
#include "call.h"

int jump(boolcg z, translation_unit ** allotherunits,commandlisttype** currentcommand,\
		translation_unit ** current_unit, int * commandnum, int * c);

#endif
