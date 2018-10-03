#ifndef VMS_H
#define VMS_H

#include "vmlib.h"
#include "pop.h"

#include "../memlib.h"
#include "../debuglib.h"
#include "../Listint.h"
#include "../interpreter.y.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>




extern typecg  *used_type1, *used_type2, *used_type3;

int run_virtual_machine(translation_unit*,translation_unit**,int);

#endif
