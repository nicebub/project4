#include "trans.h"
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
		name[a]=(char)NULL;
	}
}

char* genlabelw(char* name, int labelnum){
	char* tempstr;
	int a;
	char buf[30];
	sprintf(buf,"%d",labelnum);
	tempstr = (char*) malloc(sizeof(char)*(strlen(name)+strlen(buf)+2));
	nullout(tempstr, (int)(strlen(name)+strlen(buf)+2));
	tempstr[0]='$';
	tempstr++;
	for(a=0;a<(strlen(name));a++)
		tempstr[a]=name[a];
	tempstr= tempstr-1;
	tempstr = (char*)strcat(tempstr,buf);
	return tempstr;
}

void deletelabel(char* label){
	if(label != NULL){
		free(label);
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


char * concat(char* char1, char* char2){
	char* tempstr;
        int a;
        tempstr = (char*) malloc(sizeof(char)*(strlen(char1)+strlen(char2)+1));
	nullout(tempstr, (int)(strlen(char1)+strlen(char2)+1));
        for(a=0;a<(strlen(char1));a++)
                tempstr[a]=char1[a];
        tempstr = (char*)strcat(tempstr,char2);
        return tempstr;

}


