#ifndef _MYCGL
#define _MYCGL
#include "debuglib.h"
#define MAX_STR_CONST 25

	char string_buf[MAX_STR_CONST];
	char *string_buf_ptr;
	int warning(char*,char*);
	extern int Line_Number;
	extern char* MAIN_STRING;
	
#endif
