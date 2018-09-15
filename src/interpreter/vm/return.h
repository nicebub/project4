#ifndef RETURN_H
#define RETURN_H

#include <stdlib.h>

#include "pop.h"
#include "store.h"
#include "call.h"

extern typecg used_type7;

int returnf(translation_unit ** allotherunits,int* c, int * commandnum, translation_unit** current_unit,commandlisttype** currentcommand);

#endif
