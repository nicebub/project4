#ifndef VMS_H
#define VMS_H
#include "vmlib.h"

extern typecg  *used_type1, *used_type2, *used_type3;

extern stack vm_stack;
extern memory vm_memory;


void run_virtual_machine(translation_unit*,translation_unit**,int);

#endif
