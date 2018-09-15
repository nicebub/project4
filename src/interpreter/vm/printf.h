#ifndef MYPRINTF_H
#define MYPRINTF_H

#include <stdlib.h>
#include <stdio.h>

#include "stacklib.h"
#include "memorylib.h"
#include "call.h"

void mprintf(int count);

char * s3w(char * infmt, void ** p_array, int p_size);


#endif