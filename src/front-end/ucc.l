/*scott lorberbaum
   compilers spring 2004
	This file is the flex specificiation file for the uC language.  It will recognize and return all tokens and output if there are any errors.
*/
%{
#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "List.h"
#include "symtab.h"
#include "data.h"
#define YYSTYPE data
#include "ucc.tab.h"
#include "ucc.l.h"
#include "main.h"
#include <stdint.h>
int Line_Number = 1;
%}

%option stack
%option yylineno
%s	initial
%x	string
%x	comment

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

EQUEQU	"=="
NEQ	"\!="
LEQ	"<="
GEQ	">="
ADOF	"&"


%%
<comment>{
[^*\n]*
[^*\n]*\n 	++Line_Number;
\*+[^*/\n]*
\*+[^*/\n]*\n 	++Line_Number;
{ENDCOM} {
		BEGIN(INITIAL);}
<<EOF>> {
	error("eof terminated comment","");
	BEGIN(INITIAL);
	}
}

<string>{
\" {	BEGIN(INITIAL);
	*string_buf_ptr ='\0';
	yylval.value.svalue = (char*) strdup(string_buf);
	return(StrConstant);
	}
\n {	BEGIN(INITIAL);
	++Line_Number;
	error("unterminated string constant","");
	}

\\n {
	*string_buf_ptr++ = '\\';
	*string_buf_ptr++ = 'n';
	}
\\\" {
	*string_buf_ptr++ = '\\';
	*string_buf_ptr++ = '\"';
	}

\\[^n\\\"] {
		register int c;
		BEGIN(INITIAL);
		error("illegal character:%s in string",yytext);
		while(((c = input()) != '\"') && (c != EOF));
		if(c == EOF) error("eof terminated string","");
		}

((\\\\{SPACE}+)*|[&@$\%^/,.+'-\[\]{}();~`|=#\!:<>]*|{LETTER}|{DIGIT}|{PERCENT}|{SPACE}|{BEGCOM}|{ENDCOM}|{QUESTION})* {
	char *yptr = yytext;
	while((*yptr != (int)NULL) && *yptr)
		*string_buf_ptr++ = *yptr++;
	}

<<EOF>> {
	error("eof terminated string","");
	BEGIN(INITIAL);
	}
}


void {/*keyword void*/
	return(voidt);
	}

if {/* keyword if*/
	return(ift);
	}

return {/* keyword return*/
	return(returnt);
	}

else {/* keyword else*/
	return(elset);
	}


while {/*keyword while*/
	return(whilet);
	}

int { /*keyword int*/
	yylval.ttype = INT;
	return(intt);
	}

float {/* keyword float*/
	yylval.ttype = FLOAT;
	return(floatt);
	}

char { /* keyword char*/
	yylval.ttype = CHAR;
	return(chart);
	}

\( {
	return(lpar);
	}

\) {
	return(rpar);
	}

\{ {
	return(lcbra);
	}

\} {
	return(rcbra);
	}

\; {
	return(semi);
	}

\, {
	return(comma);
	}

\< {	yylval.value.relopvalue = LES;
	return(lesst);
	}

\> {	yylval.value.relopvalue = GRE;
	return(greatt);
	}

\= {
	return(equal);
	}

\+ {	yylval.value.addopvalue = PLS;
	return(plus);
	}

\- {	yylval.value.addopvalue = MIN;
	return(minus);
	}

\/ {	yylval.value.multopvalue = DIV;
	return(divide);
	}
\* {	yylval.value.multopvalue = MULT;
	return(star);
	}

{GEQ} {		yylval.value.relopvalue = GEQ;
		return(geq);
	}

{LEQ} {		yylval.value.relopvalue = LEQ;
		return(leq);
	}

{NEQ} {		yylval.value.eqopvalue = NEQ;
		return(neq);
	}

{EQUEQU} {	yylval.value.eqopvalue = EQEQ;
		return(equequ);
	}

{ADOF} {
		return(adof);
	}

{BEGCOM} {
		BEGIN(comment);
	}

{LETTER}({LETTER}|{DIGIT})* 	{/* identifier*/
				yylval.ttype = STR;
				yylval.value.svalue = (char *) strdup(yytext);
				return(Ident);
				}

((0|([1-9]{DIGIT}*)))(while|if|void|return|char|int|float|else|for|extern) {
									error("illegal token: %s", yytext);
							}

((0|([1-9]{DIGIT}*))(\.{DIGIT}+))(while|if|void|return|char|int|float|else|for|extern) {
									error("illegal token: %s",yytext);
									}

(0|([1-9]{DIGIT}*))({LETTER}({LETTER}|{DIGIT})*) { /* bad form*/
						error("illegal token: %s", yytext);
						}

((0|([1-9]{DIGIT}*))(\.{DIGIT}{DIGIT}*))({LETTER}({LETTER}|{DIGIT})*) {/*bad form*/
							error("illegal token: %s",yytext);
							}



(0|([1-9]{DIGIT}*))\.{DIGIT}{DIGIT}* {/*float*/
					yylval.ttype = FLOAT;
					yylval.value.fvalue=(float)atof(yytext);
					return(FloatConstant);
					}

0{DIGIT}+\.{DIGIT}+		{/* bad float*/
					error("illegal float: %s",yytext);
				}


0{DIGIT}+	{/* bad integer*/
				error("illegal integer: %s", yytext);
		}

(0|([1-9]{DIGIT}*))		{/*integer constant*/
				yylval.ttype = INT;
				yylval.value.ivalue =(int)atoi(yytext);
				return(IntConstant);
				}

{ELIP} {
	return(elip);
	}

\" {
	string_buf_ptr = string_buf;
	BEGIN(string);
	}


\n	{
	++Line_Number;
	}

[ \t\r]+	{ /*eat up tabs, whitespace */
	}

<<EOF>> {
	return '\0';
	}

. 	{
	error("illegal character: %s",yytext);
	}

%%

#ifdef DEBUG
void debugprint(char* s1, char*s2){
	if(s1!=NULL && *s1 && s2!=NULL && *s2){
//			fprintf(stderr,"Debug:::%s:%d-> %s \"%s\"\n",filename,Line_Number,s1,s2);
			fprintf(stderr,"Debug:::%s:%d yylineno-> %s \"%s\"\n",filename,yylineno,s1,s2);
	}
	else if(s1!=NULL && *s1 && s2==NULL){
//			fprintf(stderr,"Debug:::%s:%d-> %s\n",filename,Line_Number,s1);
			fprintf(stderr,"Debug:::%s:%d yylineno-> %s\n",filename,yylineno,s1);
	}
}
void debugprintd(char* s1, int s2){
	if(s1!=NULL && *s1){
//			fprintf(stderr,"Debug:::%s:%d-> %s \"%d\"\n",filename,Line_Number,s1, s2);
			fprintf(stderr,"Debug:::%s:%d yylineno-> %s \"%d\"\n",filename,yylineno,s1, s2);
	}
}

#endif
