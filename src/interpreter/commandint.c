#if defined(DEBUGON) && defined(COMMANDDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(COMMANDDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif


#include "commandint.h"


#define eprint_helper(x,filename,...) printf("%s: \033[0;31merror:\033[0m " x "\n",filename __VA_ARGS__);
#define wprint_helper(x,filename,...) printf("%s: \033[0;34mwarning:\033[0m " x "\n",filename __VA_ARGS__);
#define eprint(x, ...) eprint_helper(x, argv[0],__VA_ARGS__)
#define wprint(x, ...) wprint_helper(x, argv[0],__VA_ARGS__)


FILE *yyin_temp;
FILE *infile;


FILE* checkargs(int argc, char** argv){
	if(argc >1){																// Comparison 1 	argc > 1
		size_t in_size = strlen(argv[1]);										// Library call 1  --- strlen

		#ifdef DEBUG
		dbprint(COMMANDC,"insize =: ", 1, INT, in_size);
		#endif

		if(in_size > 4 && strcmp(&argv[1][in_size-4], ".asm")==0){				// Library call 2  --- strcmp && comparison 2 & 3 (insize,4) (strcmp(argv[1],".asm"),0)
			yyin_temp = fopen(argv[1], "r");									// Library call 3  --- fopen
			if(yyin_temp != NULL){												// Comparison 4 yyin_temp != NULL
				#ifdef DEBUG
				dbprint(COMMANDC,"Opening file for reading", 1, STR, argv[1]);
				#endif
				return yyin_temp;
			}

			else eprint("Cannot open input file %s", ,argv[1]);					// Library call 4  --- printf
		}

		else eprint("either files doesn't exist or does not end in .asm");		// Library call 5  --- printf
	}

	else eprint("no input files");												// Library call 6  --- printf

	return NULL;
}

char* openfile(int argc, char** argv){
	char * tempstr, *tempstr2;
	int a;
	int tag;
	size_t length;
	size_t argv13_length;
	infile=NULL;
	tempstr=NULL;
	tempstr2=NULL;

	argv13_length = strlen(argv[1]);											// Library call 1  --- strlen
    REQUESTMEM(tempstr,char,STR,*argv13_length+3);								// extra calculations 
	length = strlen(argv[1])-4;													// Library call 2  --- strlen
	for(a=0;a<length;a++)														// Comparison length -1 times
		tempstr[a]=argv[1][a];
	tempstr[a] = '.';
	tempstr[a+1] = 's';
	tempstr[a+2] = '\0';
	if((infile = fopen(tempstr,"w"))==NULL){									// Library call 3  --- fopen && Comparison length-th time
		eprint("Cannot open file for writing: %s", ,tempstr);					// Library call 4  --- printf
		release(tempstr, STR, tag);												// extra calculations
		tempstr=NULL;
		return NULL;
	}
	else{
	    REQUESTMEM(tempstr2, char, STR, *argv13_length+3)						// extra calculations
		strlcpy(tempstr2, argv[1],argv13_length+3);								// Library call 6  --- strlcpy
		return tempstr2;
	}
}
FILE* openfilea(char *name){
	char * tempstr;
	FILE * outfile;
	int tag;
	int a;
	size_t length;
	tempstr=NULL;
	outfile = NULL;

	length = strlen(name);														// Library call 1  --- strlen
    REQUESTMEM(tempstr, char, STR, *length+1)									// extra calculations
	for(a=0;a<(length+1);a++)													// Comparison length times
		tempstr[a]=name[a];
	tempstr[a] = '\0';
	if((outfile = fopen(tempstr,"w"))==NULL){									// Library call 2  --- fopen && comparison length +1-th time
		eprint_helper("Cannot open file for writing: %s",filename, ,tempstr);	// Library call 3  --- printf
		release(tempstr, STR, tag);												// extra calculations
		tempstr=NULL;
		return NULL;
	}
	else{
		return outfile;
	}
}
