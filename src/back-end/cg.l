/*
	Code Generator
*/
%{
#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "debuglib.h"
#include "Listcg.h"
#include "symtabcg.h"
#include "datacg.h"
#include "memlib.h"
#define YYSTYPE data
//#define YY_INPUT(buf,result,max_size)  {\
//   result = GetNextChar(buf, max_size); \
 //  if (  result <= 0  ) \
//      result = YY_NULL; \
//    }
#include "cg.tab.h"
#include "cg.l.h"
#include "main.h"
#include <stdint.h>
//extern int Line_Number;
int tag;
int check_str_length_bf_input(char* input);
char maxstrconst = (char)MAX_STR_CONST;
%}
%option noyywrap
%option nodefault
%option stack
%s	initial
%x	string

DIGIT	[0-9]
LETTER	[A-Za-z\_]
SPACE	[ ]
EOLN	\\n
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


%%

<string>{
\" {	BEGIN(INITIAL);
	if(check_str_length_bf_input("")){
		*string_buf_ptr ='\0';
		yylval.value.svalue = (char*) requestmem(strlen(string_buf)+1, STR, &tag);
			if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
		strcpy(yylval.value.svalue,string_buf);
		yylval.value.svalue[strlen(string_buf)] = '\0';
		#ifdef DEBUG
			debugprint(0,"End of string being processed - returning\n","");
		#endif
		++current_char;
		return(StrConstant);
	}
	else {
		#ifdef DEBUG
			debugprint(0,"End of string being processed - not enough room for NULL character\n","");
		#endif
		error(1,"string too large, size must be less than:", &maxstrconst);

	}
	}

\n {	BEGIN(INITIAL);
	++Line_Number;
	current_char = 1;
		#ifdef DEBUG
			debugprint(0,"Matched unallowed newling character in processing string\n","");
		#endif
	error(1,"unterminated string constant","");
	}

\\n {
	if(check_str_length_bf_input(yytext)){
		*string_buf_ptr++ = '\\';
		*string_buf_ptr++ = 'n';
		#ifdef DEBUG
			debugprint(0,"Matched character Escaped n double backslash n in processing string ",yytext);
		#endif
		current_char += 2;
	}
	else {
		register int c;
		BEGIN(INITIAL);
		current_char +=2;
		#ifdef DEBUG
			debugprint(0,"Matched character Escaped n but pattern string too large - absorbing ",yytext);
		#endif
		error(1,"string too large, size must be less than:", &maxstrconst);
		while(((c = input()) != '\"') && (c != EOF)){
			++current_char;
			#ifdef DEBUG
				debugprint(0,"Matched characters inside string but too large - absorbing ",yytext);
			#endif
			;
		}
		if(c == EOF) error(1,"eof terminated string","");
			++current_char;
	}
	}

\\\" {
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
		error(1,"string too large, size must be less than:", &maxstrconst);
		while(((c = input()) != '\"') && (c != EOF)){
			if(c == '\n') current_char = 0;
			++current_char;
			#ifdef DEBUG
				debugprint(0,"Matched characters inside string but too large - absorbing ",yytext);
			#endif
			;
		}
		if(c == EOF) error(1,"eof terminated string","");
			++current_char;
	}

	}

\\[^n\\\"] {
		register int c;
		BEGIN(INITIAL);
			current_char += 2;
		#ifdef DEBUG
	debugprint(0,"Backslash and then another character - not n, backslash, or double quotation in string Processing ",yytext);
		#endif
		error(1,"illegal character:%s a in string",yytext);
		while(((c = input()) != '\"') && (c != EOF)){
			if(c == '\n') current_char = 0;
			++current_char;
		}
		if(c == EOF) error(1,"eof terminated string","");
			++current_char;
		}

((\\\\{SPACE}+)*|[&@$\%^/,.+'-\[\]{}();~`|=#\!:<>]*|{LETTER}|{DIGIT}|{PERCENT}|{SPACE}|{BEGCOM}|{ENDCOM}|{QUESTION})* {
	if(check_str_length_bf_input(yytext)){
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
		error(1,"string too large, size must be less than:", &maxstrconst);
		while(((c = input()) != '\"') && (c != EOF)){
			if(c == '\n') current_char = 0;
			++current_char;
			#ifdef DEBUG
				debugprint(0,"Matched characters inside string but too large - absorbing ",yytext);
			#endif
			;
		}
		if(c == EOF) error(1,"eof terminated string","");
			++current_char;

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
//	ECHO;
	++current_char;
	error(1,"Characters are not allowed in string","");
	#ifdef DEBUG
		debugprint(0,"OTHER CHARACTERS while in processing a string constant\n","");
	#endif
	BEGIN(INITIAL);
	}
}



return[f]? {/* keyword return */
	#ifdef DEBUG
		debugprint(0,"Keyword return or returnf found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

alloc|enter|call {/* keyword alloc|enter|call */
	#ifdef DEBUG
		debugprint(0,"Keyword alloct, enter, or call found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}


push(s|cI|a) { /*keyword pushs, pushcI and pusha */
	#ifdef DEBUG
		debugprint(0,"Keyword pushs, pushcI, or pusha found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

popI { /* keyword popI */
	#ifdef DEBUG
		debugprint(0,"Keyword popI found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);

	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

fetch(I|R) { /* keyword fetchI or fetchR */
	#ifdef DEBUG
		debugprint(0,"Keyword fetchI or fetchR found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

store(I|R) { /* keyword storeI or storeR */
	#ifdef DEBUG
		debugprint(0,"Keyword storeI or storeR found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

setrvI { /* keyword setrvI */
	#ifdef DEBUG
		debugprint(0,"Keyword setrvI found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

leI|ltI { /* keyword leI or ltI*/
	#ifdef DEBUG
		debugprint(0,"Keyword leI or ltI found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

jump(z)? { /* keyword jumpz or jump */
	#ifdef DEBUG
		debugprint(0,"Keyword jump or jumpz found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

add(I|R) { /* keyword addI or addR */
	#ifdef DEBUG
		debugprint(0,"Keyword addI or addR found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

sub(I|R) { /* keyword subI or subR */
	#ifdef DEBUG
		debugprint(0,"Keyword subI or subR found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

mul(I|R) { /* keyword mulI or mulR */
	#ifdef DEBUG
		debugprint(0,"Keyword mulI or mulR found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

flt(b)? { /* keyword flt */
	#ifdef DEBUG
		debugprint(0,"Keyword flt found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

"int" { /* keyword int */
	#ifdef DEBUG
		debugprint(0,"Keyword int found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(CommandName);
	}

\( {
	#ifdef DEBUG
		debugprint(0,"Left Parentheses character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(lpar);
	}

\) {
	#ifdef DEBUG
		debugprint(0,"Right Parentheses character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(rpar);
	}

\{ {
	#ifdef DEBUG
		debugprint(0,"Left Curly Bracket character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(lcbra);
	}

\} {
	#ifdef DEBUG
		debugprint(0,"Right Curly Bracket character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(rcbra);
	}

\; {
	#ifdef DEBUG
		debugprint(0,"Semicolon character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(semi);
	}

\, {
	#ifdef DEBUG
		debugprint(0,"Comma character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(comma);
	}

\< {	yylval.value.relopvalue = LES;
	#ifdef DEBUG
		debugprint(0,"Less than character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(lesst);
	}

\> {	yylval.value.relopvalue = GRE;
	#ifdef DEBUG
		debugprint(0,"Greater than character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(greatt);
	}

\= {
	#ifdef DEBUG
		debugprint(0,"Equals sign character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(equal);
	}

\+ {	yylval.value.addopvalue = PLS;
	#ifdef DEBUG
		debugprint(0,"Plus sign character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(plus);
	}

\- {	yylval.value.addopvalue = MIN;
	#ifdef DEBUG
		debugprint(0,"Minus sign character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(minus);
	}

\/ {	yylval.value.multopvalue = DIV;
	#ifdef DEBUG
		debugprint(0,"Divide sign character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(divide);
	}
\* {	yylval.value.multopvalue = MULT;
	#ifdef DEBUG
		debugprint(0,"Multiplication character found with clear matching pattern ",yytext);
	#endif
	++current_char;
	return(star);
	}

{GEQ} {		yylval.value.relopvalue = GEQ;
		#ifdef DEBUG
			debugprint(0,"Greater than or equal character set found with clear matching pattern ",yytext);
		#endif
		current_char += 2;
		return(geq);
	}

{LEQ} {		yylval.value.relopvalue = LEQ;
		#ifdef DEBUG
			debugprint(0,"Less than or equal character set found with clear matching pattern ",yytext);
		#endif
		current_char += 2;
		return(leq);
	}

{NEQ} {		yylval.value.eqopvalue = NEQ;
		#ifdef DEBUG
			debugprint(0,"Not equal character set found with clear matching pattern ",yytext);
		#endif
		current_char += 2;
		return(neq);
	}

{EQUEQU} {	yylval.value.eqopvalue = EQEQ;
		#ifdef DEBUG
			debugprint(0,"Equivalence character set found with clear matching pattern ",yytext);
		#endif
		current_char += 2;
		return(equequ);
	}

{ADOF} {
		#ifdef DEBUG
			debugprint(0,"Ampersand - Address of Operator found with clear matching pattern ",yytext);
		#endif
		++current_char;
		return(adof);
	}

{DLLRSN}({LETTER}|{DIGIT})+{DIGIT}*	{ /* identifier / label */
				yylval.ttype = STR;
				yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
					if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
				strcpy(yylval.value.svalue,yytext);
					yylval.value.svalue[strlen(yytext)] = '\0';
				#ifdef DEBUG
					debugprint(0,"Identifier found with matching pattern ",yytext);
				#endif
				current_char += strlen(yytext);
				return(Ident);
				}


main { /* last function - the call to main */
		int tag;
		yylval.ttype= STR;
		yylval.value.svalue = (char*) requestmem(strlen(MAIN_STRING)+1, STR, &tag);
			if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
		strcpy(yylval.value.svalue,yytext);
			yylval.value.svalue[strlen("main")] = '\0';
		#ifdef DEBUG
			debugprint(0,"Main function found - assumed near end of file ",yylval.value.svalue);
		#endif
		current_char += strlen(yytext);
		return(maint);
	}
((0|([1-9]{DIGIT}*)))(alloc|enter|pushs|pusha|pushcI|"return"|call) {
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s", yytext);
							}

((0|([1-9]{DIGIT}*)))("int"|popI|fetchI|fetchR|storeI|storeR|setrvI)	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s", yytext);
							}
((0|([1-9]{DIGIT}*)))(leI|ltI|jumpz|jump|addI|addR|subI|subR)	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s", yytext);
							}
((0|([1-9]{DIGIT}*)))(mulI|mulR|flt|fltb)	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s", yytext);
							}


((0|([1-9]{DIGIT}*))(\.{DIGIT}+))(alloc|enter|pushs|pusha|pushcI|"return")	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s",yytext);
									}
((0|([1-9]{DIGIT}*))(\.{DIGIT}+))(call|int|popI|fetchI|fetchR|storeI|storeR|setrvI)	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s",yytext);
									}
((0|([1-9]{DIGIT}*))(\.{DIGIT}+))(leI|ltI|jumpz|jump|addI|addR|subI|subR|mulI)	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s",yytext);
									}
((0|([1-9]{DIGIT}*))(\.{DIGIT}+))(mulR|flt|fltb)	{
									#ifdef DEBUG
										debugprint(0,"Character pattern found is not in the language ",yytext);
									#endif
									current_char += strlen(yytext);
									error(1,"illegal token: %s",yytext);
									}

(0|([1-9]{DIGIT}*))({LETTER}({LETTER}|{DIGIT})*) { /* bad form*/
							#ifdef DEBUG
								debugprint(0,"Character pattern found is not in the language ",yytext);
							#endif
							current_char += strlen(yytext);
						error(1,"illegal token: %s", yytext);
						}

((0|([1-9]{DIGIT}*))(\.{DIGIT}{DIGIT}*))({LETTER}({LETTER}|{DIGIT})*) {/*bad form*/
							#ifdef DEBUG
								debugprint(0,"Character pattern found is not in the language ",yytext);
							#endif
							current_char += strlen(yytext);
							error(1,"illegal token: %s",yytext);
							}



(0|([1-9]{DIGIT}*))\.{DIGIT}{DIGIT}* {/*float*/
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

(0|([1-9]{DIGIT}*))		{/*integer constant*/
				yylval.ttype = INT;
				yylval.value.ivalue =(int)atoi(yytext);
				#ifdef DEBUG
					debugprint(0,"Integer found unchecked bounds ",yytext);
				#endif
				current_char += strlen(yytext);
				return(IntConstant);
				}

{ELIP} {
	#ifdef DEBUG
		debugprint(0,"Elipses (...) found ",yytext);
	#endif
	current_char += strlen(yytext);
	return(elip);
	}

\" {
	string_buf_ptr = string_buf;
	*string_buf_ptr = '\0';
	++current_char;
	BEGIN(string);
	#ifdef DEBUG
		debugprint(0,"Double Quotes Found: Beginning of a string assumed ",yytext);
	#endif
	}


\n	{
	#ifdef DEBUG
		debugprint(0,"Found a newline character - increasing internal counter","");
	#endif
	current_char = 1;
	++Line_Number;
	return newlinet;
	}

[ \t\r]+	{ /*eat up tabs, whitespace */
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
int check_str_length_bf_input(char* input){
	if((int)strlen(string_buf_ptr)+(int)strlen(input) < MAX_STR_CONST){
		return 1;
	}
	return 0;
}

/*returnf {/* keyword returnf 
	#ifdef DEBUG
		debugprint(0,"Keyword returnf found with clear matching pattern ",yytext);
	#endif
	current_char += strlen(yytext);
	yylval.ttype = COMMAND;
	yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
		if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
	strcpy(yylval.value.svalue,yytext);
		yylval.value.svalue[strlen(yytext)] = '\0';
	return(returnft);
	}*/

	/*{DLLRSN}({DIGIT})+	{ /* created label 
										yylval.ttype = STR;
										yylval.value.svalue = (char*) requestmem(strlen(yytext)+1, STR, &tag);
											if(yylval.value.svalue == NULL){ error(1,"OUT OF MEMORY",""); exit(-1);}
										strcpy(yylval.value.svalue,yytext);
											yylval.value.svalue[strlen(yytext)] = '\0';
										#ifdef DEBUG
											debugprint(0,"Created Label found with matching pattern ",yytext);
										#endif
										current_char += strlen(yytext);
										return(labelt);
										}
	*/
