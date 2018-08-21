#ifndef _MYCGL
#define _MYCGL
#define MAX_STR_CONST 25
	char string_buf[MAX_STR_CONST];
	char *string_buf_ptr;
	void debugprint(char*,char*);
	int error(char*,char*);
	int warning(char*,char*);
	
#endif
