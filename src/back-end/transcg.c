#ifdef DEBUGON
#ifdef TRANSDEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif
#endif
#include "transcg.h"
#include "memlib.h"
#include "debuglib.h"
//#include "cg.tab.h"
//#include <stdio.h>
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
		error(TRANSC,"Can't generate a label without a name","");
		return NULL;
	}
//	tempstr = (char*) requestmem(strlen(name)+(strlen(buf)+2), STR, &tag);
	tempstr = (char*) requestmem(strlen(name)+2, STR, &tag);
	if(tempstr == NULL){ 
		error(TRANSC,"OUT OF MEMORY",""); exit(-1);
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
	char * trunc_name;
	trunc_name = NULL;
	trunc_name = name;
	if(strcmp("_main1", name) == 0){
		trunc_name = cut_str(name);
	}
	fprintf(infile, "%s:					##  LABEL\n", trunc_name);
//	fprintf(infile, "\n", name);
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
	fprintf(infile, "# File: %s\n",filename);
	
}
void gen_section_text(){
	fprintf(infile, "	.section __TEXT,__text,regular,pure_instructions\n");
	fprintf(infile, "	.macosx_version_min 10, 13\n");
	
}
void gen_printf_dec(){
//	fprintf(infile, "extern _printf\n");
	
}
void gen_global_main(){
#ifdef DEBUG
dbprint(TRANSC,"generating global main ",STR,"");
#endif
	
	fprintf(infile, "	.globl _main		## -- BEGIN MAIN FUNCTION\n");
#ifdef DEBUG
dbprint(TRANSC,"generating p2aligning ",STR,"");
#endif
	fprintf(infile, "	.p2align 	4, 0x90\n");
#ifdef DEBUG
dbprint(TRANSC,"done ",STR,"");
#endif
	
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
	fprintf(infile, "	pushq %%rbp\n");
	fprintf(infile, "	.cfi_def_cfa_offset 16\n");
	fprintf(infile, "	.cfi_offset %%rbp, -16\n");
	fprintf(infile, "	movq	%%rsp, %%rbp\n");
	fprintf(infile, "	.cfi_def_cfa_register %%rbp\n");
//	fprintf(infile, "	movq	%%rsp, %%rbp\n");
//	fprintf(infile, "	.cfi_def_cfa_register %%rbp\n");
}

void gen_reverse_stack(){
	fprintf(infile,"	movl $0, -4(%%rbp)\n");
	fprintf(infile, "	popq %%rbp\n");
//	fprintf(infile, "	mov esp, ebp\n");
//	fprintf(infile, "	pop ebp\n");
//	fprintf(infile, "mov eax, 0\n");
//	fprintf(infile, "ret\n");
}

void gen_func_prolog(){
	fprintf(infile, "	.cfi_startproc\n");
//	fprintf(infile, "	pop ebx\n");
//	fprintf(infile, "	pop ebx\n");
	
}
void gen_func_epilog(){
	fprintf(infile, "	retq\n");
	fprintf(infile, "	.cfi_endproc\n");
	fprintf(infile, "					#END FUNCTION\n");
//	fprintf(infile, "	pop ebx\n");
	
}

void gen_end_prog(){
	fprintf(infile, ".subsections_via_symbols\n");
}

void check_and_gen_if_main(char * name){
	if(name != NULL){
		if(strcmp("$main1", name) == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "matched main func ", STR, name);
			#endif
			gen_global_main();
		}
	}
}

void gen_command(commandlisttype * inList){
	ListC * paramlist;
	char * name;
	int length;
	name = NULL;
	
	paramlist = NULL;
	
	if(inList !=NULL){
		length = inList->length;
		name = inList->name;
		paramlist = inList->paramlist;
		
		if(strcmp(name,"alloc") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an alloc", STR, "");
			#endif
			if(paramlist != NULL && length == 1){
				if(paramlist->list->argtype[0] == INT && paramlist->list->int_val[0] != 0){
					#ifdef DEBUG
					dbprint(TRANSC, "first arg value ", INT, &paramlist->list->int_val[0]);
					#endif
					if (paramlist->list->int_val[0] > 0){
						fprintf(infile, "	subq $%d, %%rsp\n", 4*paramlist->list->int_val[0]);
						currentsp = 4*paramlist->list->int_val[0];
						local_total = currentsp;
					}
					else {
						
					}
					
				}
				else{
					#ifdef DEBUG
					dbprint(TRANSC, "first arg value that is either not an int or equal to 0 ", INT, &paramlist->list->int_val[0]);
					dbprint(TRANSC,"either not an int or equal to 0", STR, "");
					#endif						

					if( paramlist->list->int_val[0] == 0){
					}
				}
			}
			else{
				if(paramlist != NULL){
				#ifdef DEBUG
					dbprint(TRANSC, "first arg value and more arguments ", INT, &paramlist->list->int_val[0]);
//				fprintf(stdout,"parameter list either null or length > 1\n");
				#endif
				}
			}
			
		}
		else if(strcmp(name,"pushcI") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an pushcI", STR, "");
			#endif
			if(paramlist != NULL && length == 1){
				if(paramlist->list->argtype[0] == INT && paramlist->list->int_val[0] != 0){
					#ifdef DEBUG
					dbprint(TRANSC,"so far so good", STR, "");
					#endif
					if(currentsp != 0){
						fprintf(infile,"	movl $%d, %d(%%rsp)\n", paramlist->list->int_val[0], currentsp);
						currentsp -= 4;
					}
					else{
						#ifdef DEBUG
						dbprint(TRANSC,"stack has no memory or stack pointer hasn't been moved", STR, "");
						#endif
					}
				}
				else{
					#ifdef DEBUG
					dbprint(TRANSC,"either not an int or equal to 0", STR, "");
					#endif
					if( paramlist->list->int_val[0] == 0){
						
					}
				}
			}
			else{
				#ifdef DEBUG
				dbprint(TRANSC,"parameter list either null or length > 1", STR, "");
				#endif
			}
			
		}
		else if(strcmp(name,"setrvI") == 0){
			listnodeC * templistn;
			templistn = NULL;
			#ifdef DEBUG
			dbprint(TRANSC, "found an setrvI", STR, "");
			#endif
			if(paramlist == NULL && length == 0){
				currentsp += 4;
				fprintf(infile, "	movl %d(%%rsp), %%eax\n", currentsp);
//				fprintf(infile, "	movl (%%eax), %%eax\n");
//				fprintf(infile, "	addq $%d, %%rsp\n", currentsp);

//				fprintf(infile,"	xorl %%eax, %%eax\n");
			}
			else if( paramlist != NULL && length == 0){
			}
			else if (length > 0){
				#ifdef DEBUG
				dbprint(TRANC,"length > 0", STR, "");
				#endif
				if(paramlist !=NULL){
					templistn = paramlist->list;
					if(templistn != NULL){
						switch(templistn->argtype[0]){
							case INT:
							warning(TRANSC, "setrvI takes an integer as a parameter, instead found a ", typecg_strings[templistn->argtype[0]]);
							break;

							case FLOAT:
							case STR:
							default: 
							warning(TRANSC, "setrvI takes an integer as a parameter, instead found a ", typecg_strings[templistn->argtype[0]]);
							break;
							
						}
					}
				}
				
			}
			
		}
		else if(strcmp(name,"returnf") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an returnf", STR, "");
			#endif
			if(paramlist == NULL && length == 0){
				fprintf(infile,"	addq $%d, %%rsp\n", local_total);
				local_total = 0;
				currentsp = 0;
			}
			else{
				#ifdef DEBUG
				dbprint(TRANSC,"parameter list not null or length > 0", STR, "");
				#endif
			}
			
		}
		else if(strcmp(name,"pusha") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an pusha", STR, "");
			#endif
			if(paramlist == NULL && length == 0){
//				fprintf(infile,"	addq $%d, %%rsp\n", local_total);
				local_total = 0;
				currentsp = 0;
			}
			else{
				#ifdef DEBUG
				dbprint(TRANSC,"parameter list not null or length > 0", STR, "");
				#endif
			}			
			
		}
		else if(strcmp(name,"popI") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an popI", STR, "");
			#endif
			if(paramlist == NULL && length == 0){
//				fprintf(infile,"	addq $%d, %%rsp\n", local_total);
				local_total = 0;
				currentsp = 0;
			}
			else{
			#ifdef DEBUG
				dbprint(TRANSC,"parameter list not null or length > 0", STR, "");
			#endif
			}
			
		}	
		else if(strcmp(name,"storeI") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an storeI", STR, "");
			#endif
			if(paramlist == NULL && length == 0){
//				fprintf(infile,"	addq $%d, %%rsp\n", local_total);
				local_total = 0;
				currentsp = 0;
			}
			else{
				#ifdef DEBUG
				dbprint(TRANC,"parameter list not null or length > 0", STR, "");
				#endif
			}
			
		}	
		else if(strcmp(name,"fetchI") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an fetchI", STR, "");
			#endif
			if(paramlist == NULL && length == 0){
//				fprintf(infile,"	addq $%d, %%rsp\n", local_total);
				local_total = 0;
				currentsp = 0;
			}
			else{
				#ifdef DEBUG
				dbprint(TRANSC,"parameter list not null or length > 0", STR, "");
				#endif
			}
			
		}	
		else if(strcmp(name,"addI") == 0){
			#ifdef DEBUG
			dbprint(TRANSC, "found an addI", STR, "");
			#endif
			if(paramlist == NULL && length == 0){
//				fprintf(infile,"	addq $%d, %%rsp\n", local_total);
//				local_total = 0;
//				currentsp = 0;
			}
			else{
				#ifdef DEBUG
				dbprint(TRANSC,"parameter list not null or length > 0", STR, "");
				#endif
			}
			
		}	
/*		else if(strcmp(name,"pusha") == 0){
			
		}	
		else if(strcmp(name,"pusha") == 0){
			
		}	
		else if(strcmp(name,"pusha") == 0){
			
		}	
		else if(strcmp(name,"pusha") == 0){
			
		}*/	
		
	}
}

void gen_run_commands(commandList* inList){
	if(inList != NULL){
		commandList * templist;
		commandlisttype* temptype;
		temptype = NULL;
		templist = NULL;
		templist = inList;
		temptype = inList->list;
		if(temptype!=NULL){
//			temptype = inList->list;
			for(int i = 0; i < templist->listsize; i++){
				#ifdef DEBUG
				dbprint(TRANSC,"Generating Command ",INT, &i);
				#endif
				
				gen_command(temptype);
					
				temptype = temptype->nextcommand;
			}
		}
	}
}


char* cut_str(char* inchar){
	char* tempstr;
	int a;
	tempstr = NULL;
	if( inchar == NULL) return NULL;
	tempstr = (char*) requestmem(strlen(inchar),STR, &tag);
	if(tempstr == NULL){ error(TRANSC,"OUT OF MEMORY",""); exit(-1);}
	nullout(tempstr, (int)(strlen(inchar)));
    for(a=0;a<(strlen(inchar) - 1);a++)
            tempstr[a]=inchar[a];
	tempstr[a] = '\0';
    return tempstr;
}

char * concat(char* char1, char* char2){
	char* tempstr;
	tempstr = NULL;
        int a;
        //tempstr = (char*) malloc(sizeof(char)*(strlen(char1*)+strlen(char2)+1));
		tempstr = (char*) requestmem(strlen(char1)+strlen(char2)+1, STR, &tag);
		if(tempstr == NULL){ error(TRANSC,"OUT OF MEMORY",""); exit(-1);}
		nullout(tempstr, (int)(strlen(char1)+strlen(char2)+1));
        for(a=0;a<(strlen(char1));a++)
                tempstr[a]=char1[a];
        tempstr = (char*)strcat(tempstr,char2);
        return tempstr;

}


