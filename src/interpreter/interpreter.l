/*
	Intermediate code interpreter
*/
%{
//    #define LEXERDEBUG
#if defined(DEBUGON) && defined(LEXERDEBUG) && !defined(DEBUG)
#define DEBUG
#define YYDEBUG 1
//extern int yydebug = 1;
#endif

#if defined(DEBUGON) && !defined(LEXERDEBUG) && defined(DEBUG)
#undef DEBUG
//#define YYDEBUG 1
//extern int yydebug = 1;
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
//#define YYDEBUG 1
//extern int yydebug = 1;
#endif

#include "debuglib.h"
#include "Listint.h"
#include "dataint.h"
#include "memlib.h"
#include "interpreter.tab.h"
#include "interpreter.l.h"

char string_buf[MAX_STR_CONST];
char *string_buf_ptr;

extern int Line_Number;
extern int current_char;
//#include "symtabint.h"

//#define YY_INPUT(buf,result,max_size)  {\
//   result = GetNextChar(buf, max_size); \
 //  if (  result <= 0  ) \
//      result = YY_NULL; \
//    }

#define check_str_length_bf_input(input)  ((int)strlen(string_buf_ptr)+(int)strlen(input) < MAX_STR_CONST ? 1 : 0 )
#define ABSORB_CHARS	{ while(((c = input()) != '\"') && (c != EOF)){ \
			if(c == '\n') current_char = 0; \
			++current_char; \
			; \
			} \
		if(c == EOF) error(1,"eof terminated string",""); \
			++current_char; }
			
#define DUPLICATE_YYTEXT(input) {	REQUESTMEM(yylval.value.svalue, char, STR, *string_length+1) \
		strlcpy(yylval.value.svalue,input,string_length+1); \
		yylval.value.svalue[string_length] = '\0'; \
}

extern YYSTYPE yylval;

%}
%option noyywrap
%option nodefault
%option stack
%s	initial
%x	string

DIGIT	[0-9]
NZDIGIT [1-9]
LETTER	[A-Za-z\_]
SPACE	[ ]
EOLN	\\n
NEWLINE \n
WHITESPACE [ \t\r]+
DBLQUOTE \"
BACKSLASH \\
DBLBACKSLASH \\\\
ILLEGAL_BACKSLASH [^n\\\"]
PERCENT	\%
QUESTION \?
NULLS	\\0
BEGCOM	"\/\*"
ENDCOM	"\*\/"
ELIP	"..."
DLLRSN	"$"
EQUEQU	"=="
NEQ	"\!="
LEQ	"<="
GEQ	">="
ADOF	"&"
CHARS_SET1 \(|\)|\{|\}|\-|\+|\=|\<|\>|\*|\/|\;|\,
COMMAND_SET1 alloc|returnf|return|enter|call|push(s|cI|cR|a|ga)|popI|fltb|flt|intb|int|jumpz|jump
COMMAND_SET2 (store|setrv|neg|mul|div|fetch|add|sub|lt|gt|eq|ne|le|ge)(I|R)
VARIOUS_CHARS [&@$\%^/,.+'-\[\]{}();~`|=#\!:<>]

%%

<string>{
{DBLQUOTE} {	

	BEGIN(INITIAL);

	if(check_str_length_bf_input("")){

		size_t string_length = strlen(string_buf);
		*string_buf_ptr ='\0';
 
		DUPLICATE_YYTEXT(string_buf)

		#ifdef DEBUG
			dbprint(0,"End of string being processed - returning with ",1, STR, string_buf);
		#endif

		++current_char;
		return(StrConstant);
	}
	else {

		#ifdef DEBUG
			debugprint(0,"End of string being processed - not enough room for NULL character\n","");
		#endif

		error(1,"string too large, size must be less than:", (char*)(int[]) { MAX_STR_CONST });

	}
	}

{NEWLINE} {	

	BEGIN(INITIAL);
	++Line_Number;
	current_char = 1;

		#ifdef DEBUG
			debugprint(0,"Matched unallowed newling character in processing string\n","");
		#endif

	error(1,"unterminated string constant","");
	}

{EOLN} {
	if(check_str_length_bf_input(yytext)){

		*string_buf_ptr++ = '\n';
//		*string_buf_ptr++ = 'n';

		#ifdef DEBUG
			debugprint(0,"Matched character Escaped n double backslash n in processing string ",yytext);
		#endif

		current_char += 1;
	}
	else {
		
		register int c;
		BEGIN(INITIAL);
		current_char +=1;

		#ifdef DEBUG
			debugprint(0,"Matched character Escaped n but pattern string too large - absorbing ",yytext);
		#endif
		error(1,"string too large, size must be less than:", (char*)(int[]) { MAX_STR_CONST });
		
		ABSORB_CHARS
		
	}
	}

{BACKSLASH}{DBLQUOTE} {
	if(check_str_length_bf_input(yytext)){

		*string_buf_ptr++ = '\\';
		*string_buf_ptr++ = '\"';

		current_char += 2;

		#ifdef DEBUG
			debugprint(0,"Double Escaped Double Quote found - backslash backslash backslash doublequote processing string ",yytext);
		#endif
	}
	else {

		register int c;
		BEGIN(INITIAL);
		current_char += 2;

		#ifdef DEBUG
			debugprint(0,"Matched string pattern but string too large - absorbing ",yytext);
		#endif

		error(1,"string too large, size must be less than:", (char*)(int[]) { MAX_STR_CONST });
		
		ABSORB_CHARS
		
	}

	}

{BACKSLASH}{ILLEGAL_BACKSLASH} {

		register int c;
		BEGIN(INITIAL);
		current_char += 2;

		#ifdef DEBUG
			debugprint(0,"Backslash and then another character - not n, backslash, or double quotation in string Processing ",yytext);
		#endif

		error(1,"illegal character:%s a in string",yytext);

		ABSORB_CHARS

			
		}

(({DBLBACKSLASH}{SPACE}+)*|{VARIOUS_CHARS}*|{LETTER}|{DIGIT}|{PERCENT}|{SPACE}|{BEGCOM}|{ENDCOM}|{QUESTION})* {
	if(check_str_length_bf_input(yytext+1)){

		char *yptr = yytext;

		while((yptr != NULL) && *yptr){
			*string_buf_ptr++ = *yptr++;
			++current_char; 
		}

		#ifdef DEBUG
			debugprint(0,"Matching string pattern to current string ",yytext);
		#endif
	}
	else {
		
		register int c;
		BEGIN(INITIAL);
		current_char += strlen(yytext);

		#ifdef DEBUG
			debugprint(0,"Matched string pattern but string too large - absorbing ",yytext);
		#endif

		error(1,"string too large, size must be less than:", (char*)(int[]) { MAX_STR_CONST });

		ABSORB_CHARS

	}

	}

<<EOF>> {
	error(1,"eof terminated string","");

	#ifdef DEBUG
		debugprint(0,"END-OF-FILE character matched while in processing a string constant\n","");
	#endif

	BEGIN(INITIAL);
	}
. {

	BEGIN(INITIAL);
	++current_char;

	error(1,"Characters are not allowed in string","");

	#ifdef DEBUG
		debugprint(0,"OTHER CHARACTERS while in processing a string constant\n","");
	#endif
	}
}



{COMMAND_SET1}|{COMMAND_SET2} {/* keywords  */

	size_t string_length = strlen(yytext);

	#ifdef DEBUG
		debugprint(0,"Keyword found with clear matching pattern ",yytext);
	#endif

	current_char += string_length;
	yylval.ttype = COMMAND;

	DUPLICATE_YYTEXT(yytext)

	return(CommandName);
	}


{CHARS_SET1} {
	#ifdef DEBUG
		debugprint(0,"Character found with clear matching pattern ",yytext);
	#endif

	++current_char;
	#ifdef DEBUG
	dbprint(0,"yytext as an INT ", 1, INT, yytext);
	dbprint(0,"yytext as an STR ", 1, STR, yytext);
	#endif	

		if(strcmp(yytext,",") == 0)	return(comma);
	
}

{GEQ}|{LEQ}|{NEQ}|{EQUEQU} {
}

{ADOF} {/*
		#ifdef DEBUG
			debugprint(0,"Ampersand - Address of Operator found with clear matching pattern ",yytext);
		#endif

		++current_char;
		return(adof);*/
	}

{DLLRSN}({LETTER}|{DIGIT})+{DIGIT}*	{ /* identifier / label */

				size_t string_length = strlen(yytext);;
				yylval.ttype = STR;
				#ifdef DEBUG
					debugprint(0,"Identifier found with matching pattern ",yytext);
				#endif

				DUPLICATE_YYTEXT(yytext)


				current_char += string_length;
				return(Ident);
				}


main { /* last function - the call to main */
		int tag;
		int string_length = strlen("main");
		yylval.ttype= STR;

		DUPLICATE_YYTEXT("main")
		
		#ifdef DEBUG
			debugprint(0,"Main function found - assumed near end of file ",yylval.value.svalue);
		#endif

		current_char += string_length;
		return(maint);
	}

((0|({NZDIGIT}{DIGIT}*)))({COMMAND_SET1}|{COMMAND_SET2}) {
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif

									current_char += strlen(yytext);
									error(1,"illegal token: %s", yytext);
							}

((0|({NZDIGIT}{DIGIT}*))(\.{DIGIT}+))({COMMAND_SET1}|{COMMAND_SET2})	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif

									current_char += strlen(yytext);
									error(1,"illegal token: %s",yytext);
									}

(0|({NZDIGIT}{DIGIT}*))({LETTER}({LETTER}|{DIGIT})*) { /* bad form*/
							#ifdef DEBUG
								debugprint(0,"Character pattern found is not in the language ",yytext);
							#endif

							current_char += strlen(yytext);
							error(1,"illegal token: %s", yytext);
						}

((0|({NZDIGIT}{DIGIT}*))(\.{DIGIT}{DIGIT}*))({LETTER}({LETTER}|{DIGIT})*) {/*bad form*/
							#ifdef DEBUG
								debugprint(0,"Character pattern found is not in the language ",yytext);
							#endif

							current_char += strlen(yytext);
							error(1,"illegal token: %s",yytext);
							}



(0|({NZDIGIT}{DIGIT}*))\.{DIGIT}{DIGIT}* {/*float*/
					yylval.ttype = FLOAT;
					yylval.value.fvalue=(float)atof(yytext);

					#ifdef DEBUG
						debugprint(0,"Float found unchecked bounds ",yytext);
					#endif

					current_char += strlen(yytext);
					return(FloatConstant);
					}

0{DIGIT}+\.{DIGIT}+		{/* bad float*/
					#ifdef DEBUG
					debugprint(0,"Floats cannot start with a 0 ",yytext);
					#endif

					current_char += strlen(yytext);
					error(1,"illegal float: %s",yytext);
				}


0{DIGIT}+	{/* bad integer*/
				#ifdef DEBUG
				debugprint(0,"Integers cannot start with a 0 ",yytext);
				#endif

				current_char += strlen(yytext);
				error(1,"illegal integer: %s", yytext);
		}

(0|({NZDIGIT}{DIGIT}*))		{/*integer constant*/
				yylval.ttype = INT;
				yylval.value.ivalue =(int)atoi(yytext);

				#ifdef DEBUG
					debugprint(0,"Integer found unchecked bounds ",yytext);
				#endif

				current_char += strlen(yytext);
				return(IntConstant);
				}

{ELIP} {/*
	#ifdef DEBUG
		debugprint(0,"Elipses (...) found ",yytext);
	#endif

	current_char += strlen(yytext);
	return(elip);*/
	}

{DBLQUOTE} {
	string_buf_ptr = string_buf;
	*string_buf_ptr = '\0';

	++current_char;
	BEGIN(string);
	
	#ifdef DEBUG
		debugprint(0,"Double Quotes Found: Beginning of a string assumed ",yytext);
	#endif
	}


{NEWLINE}	{

	#ifdef DEBUG
		debugprint(0,"Found a newline character - increasing internal counter","");
	#endif

	current_char = 1;
	++Line_Number;
	return newlinet;
	}

{WHITESPACE}	{ /*eat up tabs, whitespace */

	#ifdef DEBUG
		debugprint(0,"Getting rid of other whitespace","");
	#endif

	current_char += strlen(yytext);
	}


.	{

	#ifdef DEBUG
		debugprint(0,"Solo Character found ",yytext);
	#endif

	current_char += strlen(yytext);

	error(1,"illegal character: %s b",yytext);
	}
	
<<EOF>> {
		#ifdef DEBUG
			debugprint(0,"Found the END-OF-FILE character - assuming end of actual file","");
		#endif

		return '\0';
		}

%%
