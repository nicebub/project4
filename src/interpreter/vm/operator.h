#ifndef OPERATOR_H
#define OPERATOR_H

#include "../typeint.h"
#include "../debuglib.h"
#include <stdlib.h>

#include "pop.h"
#include "push.h"

extern typecg used_type5, used_type6;

void * operate(char operator, typecg intype);

int * relationship(char *relation, typecg intype);

#endif
