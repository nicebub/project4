#ifndef _MYUCCL
#define _MYUCCL
#define MAX_STR_CONST 10000
	char string_buf[MAX_STR_CONST];
	char *string_buf_ptr;

	int error(char*,char*);
	int warning(char*,char*);
	
#endif
