#include "printf.h"

#define MAX_ARG 20

void mprintf(int count){
	
	void * p_array[MAX_ARG];
	int p_size;
	char * p_string;

	for(int b=0;b<20;b++)
		p_array[b]= NULL;

	p_size = 0;

	p_string = NULL;

		#ifdef DEBUG
	   dbprint(VMLIBC,"THIS IS A PRINTF",0);
		#endif

	  char * infmt;
	  void * invalue;

	  infmt = NULL;
	  invalue = NULL;

	   if(count > 1 && count < MAX_ARG){

			 	for(int i=count;i>1;i--){

				    p_array[p_size] = pop(&used_type5,3);
					p_size++;
				}

				infmt = pop(&used_type6,4);
				s3w(infmt, p_array,p_size);

		}
		else if(count == 1){

			 infmt = pop(&used_type5,3);
			 printf("%s", infmt);
	 	}
   
}


char * s3w(char * infmt, void ** p_array, int p_size){
	char * piece3;
	char *val_place;
	char   buffer[MAX_STR_CONST];
	char   valbuffer[MAX_STR_CONST];
	char  p3buffer[MAX_STR_CONST];
	char *currentchar;
	int p1_length;
	int p2_length;
	int p3_length;

	p1_length = p2_length = p3_length = 0;
	
	for(int y=0;y<MAX_STR_CONST;y++){
		buffer[y] = ' ';
		valbuffer[y] = ' ';
		p3buffer[y] = ' ';
	}

    piece3 = NULL;
	val_place = NULL;
	

	if(infmt !=NULL && p_size >0){

		currentchar = &infmt[0];
		for(int r=0;r<p_size;r++){
			while(*currentchar != '%'){

				buffer[p1_length]= *currentchar;
				p1_length++;
				currentchar++;

			}
			buffer[p1_length] = '\0';
			printf("%s",buffer);
			currentchar++;
			

			switch(*currentchar){
				case 'd':
						sprintf(valbuffer,"%d",*(int*) p_array[r]);
						
						break;

				case 'f':
						sprintf(valbuffer,"%f",(float)*(float*) p_array[r]);
						break;
					
			
				case 's':
						sprintf(valbuffer,"%s",(char*)p_array[r]);
						break;
			
			}
			
			printf("%s", valbuffer);
			currentchar++;
		    p1_length = 0;
		}
	    p1_length = 0;
	    while(*currentchar != '\0'){
		   
		   buffer[p1_length]= *currentchar;
		   p1_length++;
		   currentchar++;
		   
	    }
	    buffer[p1_length] = '\0';
	    printf("%s",buffer);
		return NULL;
	}
	return NULL;
}
