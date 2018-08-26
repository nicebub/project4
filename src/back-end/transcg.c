#include "transcg.h"
#include "memlib.h"
#include "debuglib.h"
//#include "cg.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void initializelabel(){
	labelcounter=1;
}

int getlabel(){
	labelcounter++;
	return (labelcounter-1);
}

void nullout(char* name, int length){
	int a;
	for(a=0;a<length;a++){
		name[a]='\0';;
	}
}

/*char* w(char* name, int labelnum){
	char* tempstr;
	int a;
	char buf[30];
	sprintf(buf,"%d",labelnum);
	tempstr = (char*) malloc(sizeof(char)*(strlen(name)+strlen(buf)+2));
	nullout(tempstr, (int)(strlen(name)+strlen(buf)+2));
	tempstr[0]='_';
	tempstr++;
	for(a=0;a<(strlen(name));a++)
		tempstr[a]=name[a];
	tempstr= tempstr-1;
	tempstr = (char*)strcat(tempstr,buf);
	return tempstr;
}
*/
char* genlabelu(char* name, int labelnum){
	char* tempstr;
	int a;
//	char buf[30];
//	sprintf(buf,"%d",labelnum);
	if(name == NULL){
		error(PARSER,"Can't generate a label without a name","");
		return NULL;
	}
//	tempstr = (char*) requestmem(strlen(name)+(strlen(buf)+2), STR, &tag);
	tempstr = (char*) requestmem(strlen(name)+2, STR, &tag);
	if(tempstr == NULL){ 
		error(PARSER,"OUT OF MEMORY",""); exit(-1);
	}
//	nullout(tempstr, (int)(strlen(name)+strlen(buf)+2));
	nullout(tempstr, (int)(strlen(name)+2));
	tempstr[0]='_';
	tempstr++;
	if(name[0] == '$'){ 
		name++;
	}
	for(a=0;a<(strlen(name));a++)
		tempstr[a]=name[a];
	tempstr= tempstr-1;
	name = name - 1;
//	tempstr = (char*)strcat(tempstr,buf);
	return tempstr;
}

void deletelabel(char* label, int tag){
//	int tag;
	if(label != NULL){
		release(label, STR, tag);
		//label=NULL;
	}
}

void gen_instr(char* name){
	fprintf(infile, "\t%s\n", name);
}

void gen_instr_I(char* name, int arg){
	fprintf(infile, "\t%s\t%d\n", name, arg);
}

void gen_instr_S(char* name, char* inS){
	if(strcmp(name,"jump")==0 || strcmp(name,"jumpz")==0)
		fprintf(infile,"\t%s\t%s\n",name,inS);
	else fprintf(infile,"\t%s\t\"%s\"\n",name,inS);
}

void gen_label(char* name){
	fprintf(infile, "%s\n", name);
}

void gen_instr_F(char* name, float arg){
	fprintf(infile, "\t%s\t%f\n", name, arg);
}

void gen_call(char* funcname, int numargs){
	fprintf(infile, "\tcall\t%s, %d\n",funcname,numargs);
}

void gen_instr_tI(char * name, int arg1, int arg2){
	fprintf(infile, "\t%s\t%d, %d\n",name, arg1, arg2);
}

void gen_filename_comment(){
	fprintf(infile, "; File: %s\n",filename);
	
}
void gen_section_text(){
	fprintf(infile, "	.section __TEXT,__text,regular,pure_instructions\n");
	fprintf(infile, "	.macosx_version_min 10, 13\n");
	
}
void gen_printf_dec(){
//	fprintf(infile, "extern _printf\n");
	
}
void gen_global_main(){
	fprintf(infile, ".globl _main1		## -- BEGIN MAIN FUNCTION\n");
	
}
void gen_prolog_macro(){
	fprintf(infile, "%%macro clib_prolog 1\n");
	fprintf(infile, "	mov ebx, esp\n");
	fprintf(infile, "	and esp, 0xFFFFFFF0\n" );
	fprintf(infile, "	sub esp, 12\n");
	fprintf(infile, "	push ebx\n" );
	fprintf(infile, "	sub esp, %%1\n");
	fprintf(infile, "%%endmacro\n");
}

void gen_epilog_macro(){

/*	char *str1[] = { 
		"%%macro clib_epilog 1\n", 
		"	add esp, %1\n",
		 "	pop ebx\n", 
		 "	mov esp, ebx\n",
		 "%%end macro\n" 
	 };
	 char * temp;
	 temp = NULL;*/
	 
//	char str2[] = "	add esp, %1\n";
//	char str3[] = "	pop ebx\n";
//	char str4[] = "	mov esp, ebx\n";
		fprintf(infile, "%%macro clib_epilog 1\n");
		fprintf(infile, "	add esp, %%1\n");
		fprintf(infile, "	pop ebx\n" );
		fprintf(infile, "	mov esp, ebx\n");
		fprintf(infile, "%%end macro\n");
}

void gen_set_stack(){
	fprintf(infile, "	push ebp\n");
	fprintf(infile, "	mov ebp, esp\n");
	fprintf(infile, "	push ebx\n");
	
}

void gen_reverse_stack(){
	fprintf(infile, "	pop ebx\n");
	fprintf(infile, "	mov esp, ebp\n");
	fprintf(infile, "	pop ebp\n");
//	fprintf(infile, "mov eax, 0\n");
//	fprintf(infile, "ret\n");
}

void gen_func_prolog(){
	fprintf(infile, "	.cfi_startproc\n");
//	fprintf(infile, "	pop ebx\n");
//	fprintf(infile, "	pop ebx\n");
	
}
void gen_func_epilog(){
	fprintf(infile, "	.cfi_endproc\n");
	fprintf(infile, "					END FUNCTION\n");
//	fprintf(infile, "	pop ebx\n");
	
}

void gen_end_prog(){
	fprintf(infile, ".subsections_via_symbols\n");
}

char * concat(char* char1, char* char2){
	char* tempstr;
	tempstr = NULL;
        int a;
        //tempstr = (char*) malloc(sizeof(char)*(strlen(char1*)+strlen(char2)+1));
		tempstr = (char*) requestmem(strlen(char1)+strlen(char2)+1, STR, &tag);
		if(tempstr == NULL){ error(PARSER,"OUT OF MEMORY",""); exit(-1);}
		nullout(tempstr, (int)(strlen(char1)+strlen(char2)+1));
        for(a=0;a<(strlen(char1));a++)
                tempstr[a]=char1[a];
        tempstr = (char*)strcat(tempstr,char2);
        return tempstr;

}


