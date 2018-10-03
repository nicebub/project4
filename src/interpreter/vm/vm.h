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
    { LTI,		 "ltI" },
    { LTR,		 "ltR" },
    { LEI,		 "leI" },
    { LER,		 "leR" },
    { GTI,		 "gtI" },
    { GTR,		 "gtR" },
    { GEI,		 "geI" },
    { GER,		"geR" },
    { JUMP, 	"jump" },
    { JUMPZ, 	"jumpz" },
    { MULI, 	"mulI" },
    { MULR, 	"mulR" },
    { DIVI, 	"divI" },
    { DIVR, 	"divR" },
    { ADDI, 	"addI" },
    { ADDR, 	"addR" },
    { SUBI, 	"subI" },
    { SUBR, 	"subR" },
    { FLT,		"flt"  },
    { FLTB, 	"fltb" },
    { INTT, 	"int", },
    { INTTB,	"intb" },
    { EQI,		 "eqI" },
    { EQR,		 "eqR" },
    { NEI,		 "neI" },
    { NER,		 "neR" },
    { NEGI,		 "negI" },
    { NEGR,		 "negR" }
};



extern typecg  *used_type1, *used_type2, *used_type3;

int run_virtual_machine(translation_unit*,translation_unit**,int);

#endif
