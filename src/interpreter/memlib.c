#if defined(DEBUGON) && defined(MEMDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(MEMDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "memlib.h"
#include "debuglib.h"
#include "Listint.h"
#include <string.h>
#include <stdlib.h>
//#include <stdio.h>

mem_man mem_manager;
int tag;

void init_mem_man(){
	#ifdef DEBUG
	dbprint(MEMLIBC,"Initializing Memory Manager", 0);
/*	list_mem(INT);
	list_mem(FLOAT);
	list_mem(STR);*/
	list_mem(OBJ, TRUE);
	#endif
	
	mem_manager.i_num = 0;
	mem_manager.f_num = 0;
	mem_manager.g_num = 0;
	mem_manager.cs_num = 0;
	mem_manager.o_num = 0;
	mem_manager.f_amount = 0;
	mem_manager.cs_amount = 0;
	mem_manager.i_amount = 0;
	mem_manager.g_amount = 0;
	mem_manager.o_amount = 0;
	
	for(int i = 0; i < MAX_MEM;i++){
		mem_manager.intbucket[i] = NULL;
		mem_manager.charbucket[i] = NULL;
		mem_manager.floatbucket[i] = NULL;
		mem_manager.genericbucket[i] = NULL;
		mem_manager.objbucket[i] = NULL;
		mem_manager.otype[i] = OBJ;
	}
	#ifdef DEBUG
	dbprint(MEMLIBC,"Done.",0);
	#endif
}

int get_avail_mem_tag(typecg intype){
	switch(intype){
		case INT: 
					if(mem_manager.i_num < MAX_MEM)
					return mem_manager.i_num;
					else{
						for(int i = 0; i<MAX_MEM; i++){
							if(mem_manager.inthash[i] == 0) return i;
						}
						return -1;
					}
		case FLOAT: 
					if(mem_manager.f_num < MAX_MEM)
						return mem_manager.f_num;
					else{
						for(int i = 0; i<MAX_MEM; i++){
							if(mem_manager.floathash[i] == 0) return i;
						}
						return -1;
					}
		case STR:
		case CHAR:
					if(mem_manager.cs_num < MAX_MEM)
						return mem_manager.cs_num;
					else{
						for(int i = 0; i<MAX_MEM; i++){
							if(mem_manager.charhash[i] == 0) return i;
						}
						return -1;
					}
		case OBJ:
					if(mem_manager.o_num < MAX_MEM)
						return mem_manager.o_num;
					else{
						for(int i = 0; i<MAX_MEM; i++){
							if(mem_manager.objhash[i] == 0) return i;
						}
						return -1;
					}
		case GENERIC:
					if(mem_manager.g_num < MAX_MEM)
						return mem_manager.g_num;
					else{
						for(int i = 0; i<MAX_MEM; i++){
							if(mem_manager.generichash[i] == 0) return i;
						}
						return -1;
					}
					
					
		default: 
					return -1;
	}
}

void * requestmem(size_t size, typecg intype, int *tag){
	int * temp;
	char * tempchar;
	float * tempfloat;
	void * tempgeneric;
	temp = NULL;
	tempchar = NULL;
	tempfloat = NULL;
	tempgeneric = NULL;
//	#ifdef DEBUG
//	dbprint(MEMLIBC,"Requesting Memory",STR,"");
//	#endif
	if( (*tag = get_avail_mem_tag(intype)) != -1){	
		switch(intype){
			case INT:
						#ifdef DEBUG
						dbprint(MEMLIBC,"Requesting Memory for an Integer",0);
						#endif
						temp = NULL;
						mem_manager.intbucket[*tag] = (int* )malloc(sizeof(int)*size);
						temp = mem_manager.intbucket[*tag];
						mem_manager.inthash[*tag] = 1;
						mem_manager.intref[*tag] += 1;
						mem_manager.int_address[*tag] = (unsigned long) mem_manager.intbucket[*tag];
						mem_manager.i_amount += 1;
						#ifdef DEBUG
						dbprint(MEMLIBC,"Integer Address after creation",1, LONG,(void*)&mem_manager.intbucket[*tag]);
						#endif
						if(mem_manager.i_num < MAX_MEM) mem_manager.i_num += 1;
						return (void*)temp;
			case CHAR:
			case STR:
						#ifdef DEBUG
						dbprint(MEMLIBC,"Requesting Memory for a String/Character",0);
						#endif
						tempchar = NULL;
			   
						mem_manager.charbucket[*tag] = (char*)malloc(sizeof(char)*size);
						tempchar = mem_manager.charbucket[*tag];
						for(int i = 0; i< size-1;i++)
							tempchar[i]=' ';
			   			tempchar[size-1]='\0';
						mem_manager.charhash[*tag] = 1;
						mem_manager.charref[*tag] += 1;
						mem_manager.cs_amount += 1;
						mem_manager.char_address[*tag] = (unsigned long) mem_manager.charbucket[*tag];
						#ifdef DEBUG
						dbprint(MEMLIBC,"String Address after creation",1, LONG,(void*) &mem_manager.charbucket[*tag]);
						#endif
						if(mem_manager.cs_num < MAX_MEM) mem_manager.cs_num += 1;
						return (void*)tempchar;
			case FLOAT: 
						#ifdef DEBUG
						dbprint(MEMLIBC,"Requesting Memory for a Float",0);
						#endif
						tempfloat = NULL;
						mem_manager.floatbucket[*tag] = (float*)malloc(sizeof(float)*size);
						tempfloat = mem_manager.floatbucket[*tag];
						mem_manager.floathash[*tag] = 1;
						mem_manager.floatref[*tag] += 1;
						mem_manager.f_amount += 1;
						mem_manager.float_address[*tag] = (unsigned long) mem_manager.floatbucket[*tag];
						#ifdef DEBUG
						dbprint(MEMLIBC,"Float Address after creation",1, LONG,(void*) &mem_manager.floatbucket[*tag]);
						#endif
						if(mem_manager.f_num < MAX_MEM) mem_manager.f_num += 1; 
						return (void*)tempfloat;
			case GENERIC:
						#ifdef DEBUG
						dbprint(MEMLIBC,"Requesting Memory for a Generic Memory Type",0);
						#endif
						tempgeneric = NULL;
						mem_manager.genericbucket[*tag] = malloc(size);
						tempgeneric = mem_manager.genericbucket[*tag];
						mem_manager.generichash[*tag] = 1;
						mem_manager.genericref[*tag] += 1;
						mem_manager.g_amount += 1;
						mem_manager.generic_size[*tag] = size;
						mem_manager.generic_address[*tag] = (unsigned long) mem_manager.genericbucket[*tag];
						#ifdef DEBUG
						dbprint(MEMLIBC,"Generic Memory Object Address after creation",1, LONG,(void*) &mem_manager.genericbucket[*tag]);
						#endif
						if(mem_manager.g_num < MAX_MEM) mem_manager.g_num += 1;
						return tempgeneric;

						
			default:
						return NULL;
		}
	}
	else{
		#ifdef DEBUG
		dbprint(MEMLIBC,"Couldn't get a memory unit tag",1, INT, (void*)tag);
		#endif
		return NULL;
	}
}

void * requestmemobj(size_t size, typecg intype){
	//int tag;
	int otag;
	int_m * temp;
	float_m * tempfloat;
	str_m * tempstr;
	temp = NULL;
	tempfloat = NULL;
	tempstr = NULL;
//	#ifdef DEBUG
//	dbprint(MEMLIBC,"Requesting Memory for an Object",STR,"");
//	#endif

	if( (otag = get_avail_mem_tag(OBJ)) != -1){	
	
		switch(intype){
			case INT: 
						#ifdef DEBUG
						dbprint(MEMLIBC,"Requesting Memory for an Integer Object",0);
						#endif
						temp = (int_m* )malloc(sizeof(int_m));
						temp->value = NULL;
						temp->value = requestmem(size, intype, &(temp->tag));
						if( temp->tag != -1){
							temp->type = INT;
							temp->address = (unsigned long)temp->value;
							temp->o_tag = otag;
							mem_manager.objhash[otag]  = 1;
							mem_manager.obj_address[otag]  = (unsigned long) temp;
							mem_manager.objbucket[otag]  = temp;
//							#ifdef DEBUG
//							dbprint(MEMLIBC,"Object Bucket Tag", INT, tag);
//							dbprint(MEMLIBC,"Object Bucket", STR, mem_manager.objbucket[tag]);
//							#endif
							mem_manager.objref[otag] += 1;
							mem_manager.o_amount += 1;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Amount of allocated Objects", 1, INT, mem_manager.o_amount);
							#endif
							mem_manager.otype[otag] = INT;
							if(mem_manager.o_num < MAX_MEM) mem_manager.o_num += 1;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Object Tag: ", 1, INT, otag);
							//dbprint(MEMLIBC,"Object Starting Value:", INT, (void*) temp->value);
							dbprint(MEMLIBC,"Object Address:", 1, LONG, (void*) mem_manager.objbucket[otag]);
							dbprint(MEMLIBC,"Integer Tag: ", 1, INT,  temp->tag);
							dbprint(MEMLIBC,"Integer Starting Value:", 1, INT, (void*) temp->value);
							dbprint(MEMLIBC,"Integer Address:", 1, LONG, (void*) &mem_manager.intbucket[temp->tag]);
							dbprint(MEMLIBC,"",0);
							#endif
							return (void*)temp;
						}
						temp->value = NULL;
						free(temp);
						temp = NULL;
						return NULL;
			case FLOAT:
						#ifdef DEBUG
						dbprint(MEMLIBC,"Requesting Memory for a Float Object",0);
						#endif
						tempfloat = (float_m* )malloc(sizeof(float_m));
						tempfloat->value = NULL;
						tempfloat->value = requestmem(size, intype, &(tempfloat->tag));
						if( tempfloat->tag != -1){
							tempfloat->type = FLOAT;
							tempfloat->address = (unsigned long)tempfloat->value;
							tempfloat->o_tag = otag;
							mem_manager.objhash[otag]  = 1;
							mem_manager.obj_address[otag]  = (unsigned long) tempfloat;
							mem_manager.objbucket[otag]  = tempfloat;
							mem_manager.objref[otag] += 1;
							mem_manager.o_amount += 1;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Amount of allocated Objects", 1, INT,  mem_manager.o_amount);
							#endif
							mem_manager.otype[otag] = FLOAT;
							if(mem_manager.o_num < MAX_MEM) mem_manager.o_num += 1;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Object Tag: ", 1, INT, otag);
							//dbprint(MEMLIBC,"Object Starting Value:", INT, (void*) temp->value);
							dbprint(MEMLIBC,"Object Address:", 1, LONG, (void*) &mem_manager.objbucket[otag]);
							dbprint(MEMLIBC,"Float Tag: ", 1, INT, (tempfloat->tag));
							dbprint(MEMLIBC,"FLoat Starting Value:",1,  FLOAT, (void*) tempfloat->value);
							dbprint(MEMLIBC,"Float Address:", 1, LONG, (void*) &mem_manager.floatbucket[tempfloat->tag]);
							dbprint(MEMLIBC,"",0);
							#endif
							return (void*)tempfloat;
						}
						tempfloat->value=NULL;
						free(tempfloat);
						tempfloat=NULL;
						return NULL;
		
			case STR:
			case CHAR:
						#ifdef DEBUG
						dbprint(MEMLIBC,"Requesting Memory for a String/Character Object",0);
						#endif
						tempstr = (str_m* )malloc(sizeof(str_m));
						tempstr->value = NULL;
						tempstr->value = requestmem(size, intype, &(tempstr->tag));
						if( tempstr->tag != -1){
							tempstr->type = STR;
							tempstr->address = (unsigned long) tempstr->value;
							tempstr->o_tag = otag;
							mem_manager.objhash[otag]  = 1;
							mem_manager.obj_address[otag]  = (unsigned long) tempstr;
							mem_manager.objbucket[otag]  = tempstr;
							mem_manager.objref[otag] += 1;
							mem_manager.o_amount += 1;
							mem_manager.otype[otag] = STR;
							if(mem_manager.o_num < MAX_MEM) mem_manager.o_num += 1;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Object Tag: ", 1, INT, otag);
							//dbprint(MEMLIBC,"Object Starting Value:", INT, (void*) temp->value);
							dbprint(MEMLIBC,"Object Address:", 1, LONG, (void*) &mem_manager.objbucket[otag]);
							dbprint(MEMLIBC,"String/Character Tag: ", 1, INT, tempstr->tag);
							dbprint(MEMLIBC,"String/Character Starting Value:", 1, STR, (void*) tempstr->value);
							dbprint(MEMLIBC,"String/Character Address:",1,  LONG, (void*) &mem_manager.charbucket[tempstr->tag]);
							dbprint(MEMLIBC,"",0);
							#endif
							return (void*)tempstr;
						}
						tempstr->value = NULL;
						free(tempstr);
						tempstr=NULL;
						return NULL;

			default:
						return NULL;
		}	
	}
	return NULL;
}

int releaseall(){
#ifdef DEBUG
	dbprint(MEMLIBC,"Releasing All Memory",1, STR," : listing all memory before release");
//	list_mem(INT);
//	list_mem(FLOAT);
//	list_mem(STR);
#endif	
	for(int i = 0; i < mem_manager.i_num;i++){
		if(mem_manager.intbucket[i] != NULL) free(mem_manager.intbucket[i]);
		mem_manager.intbucket[i] = NULL;
		mem_manager.inthash[i] = 0;
		mem_manager.intref[i] = 0;
		mem_manager.int_address[i] = 0;
		mem_manager.i_amount -= 1;
	}
	mem_manager.i_num = 0;
	for(int i = 0; i < mem_manager.f_num;i++){
		if(mem_manager.floatbucket[i] != NULL) free(mem_manager.floatbucket[i]);
		mem_manager.floatbucket[i] = NULL;
		mem_manager.floathash[i] = 0;
		mem_manager.floatref[i] = 0;
		mem_manager.float_address[i] = 0;
		mem_manager.f_amount -= 1;
	}
	mem_manager.f_num = 0;

	for(int i = 0; i < mem_manager.g_num;i++){
		if(mem_manager.genericbucket[i] != NULL) free(mem_manager.genericbucket[i]);
		mem_manager.genericbucket[i] = NULL;
		mem_manager.generichash[i] = 0;
		mem_manager.genericref[i] = 0;
		mem_manager.generic_size[i] = 0;
		mem_manager.generic_address[i] = 0;
		mem_manager.g_amount -= 1;
	}
	mem_manager.g_num = 0;


	for(int i = 0; i < mem_manager.cs_num;i++){
		if(mem_manager.charbucket[i] != NULL) free(mem_manager.charbucket[i]);
		mem_manager.charbucket[i] = NULL;
		mem_manager.charhash[i] = 0;
		mem_manager.charref[i] = 0;
		mem_manager.char_address[i] = 0;
		mem_manager.cs_amount -= 1;
	}
	mem_manager.cs_num = 0;
/*	for(int i = 0; i < mem_manager.o_num;i++){
		if(mem_manager.objbucket[i] != NULL) free(mem_manager.objbucket[i]);
		mem_manager.objbucket[i] = NULL;
		mem_manager.objhash[i] = 0;
		mem_manager.objref[i] = 0;
		mem_manager.obj_address[i] = 0;
		mem_manager.o_amount -= 1;
	}
	mem_manager.o_num = 0;*/
#ifdef DEBUG
	dbprint(MEMLIBC,"Release All Memory Checking for errors",0);
//	list_mem(INT);
//	list_mem(FLOAT);
//	list_mem(STR);
#endif
		
	return 0;
}

int release(void * mem, typecg intype, int tag){
	int * temp;
	char * tempchar;
	float * tempfloat;
	void * tempgeneric;
	temp = NULL;
	tempchar = NULL;
	tempfloat = NULL;
	tempgeneric = NULL;
	
	if(mem !=NULL){
		if(tag > MAX_MEM){
				#ifdef DEBUG
				dbprint(MEMLIBC,"tag greater than MAX_MEM", 1, INT, tag);
				#endif	
				 return -1;
			 }
			switch(intype){
				case INT: 	
							#ifdef DEBUG
							dbprint(MEMLIBC,"Releasing Integer Memory with tag", 1, INT, tag);
							//list_i_mem();
							#endif	
							if((tag > mem_manager.i_num  && tag < MAX_MEM) || tag < 0){
								#ifdef DEBUG
								dbprint(MEMLIBC,"tag greater than i_num or tag < 0", 1, INT, tag);
								#endif	
								 return -1;
							 }
							if(mem_manager.intbucket[tag] != NULL){
								if(mem_manager.intref[tag] == 1){ 
									free(mem_manager.intbucket[tag]);
									mem_manager.intbucket[tag] = NULL;
									mem_manager.intref[tag] = 0;
									mem_manager.inthash[tag] = 0;
									mem_manager.int_address[tag] = 0;
									mem_manager.i_amount -= 1;
									if(tag == mem_manager.i_num -1) mem_manager.i_num -=1;
								}
								else if(mem_manager.intref[tag] > 1){
									#ifdef DEBUG
									dbprint(MEMLIBC,"Reducing Reference Count from ",1, INT,mem_manager.intref[tag]);
									#endif
									 mem_manager.intref[tag] -= 1;
								}
								 else{
									 ;
								}
							}
							#ifdef DEBUG
							dbprint(MEMLIBC,"Checking for errors on release",0);
							//list_i_mem();
							#endif
							return 0;
				case CHAR:
				case STR:
							#ifdef DEBUG
							dbprint(MEMLIBC,"Releasing String/Character Memory with tag", 1, INT, tag);
							//list_cs_mem();
							#endif	
							if((tag > mem_manager.cs_num && tag < MAX_MEM) || tag < 0) return -1;
							if(mem_manager.charbucket[tag] != NULL){
								if(mem_manager.charref[tag] == 1){ 
									free(mem_manager.charbucket[tag]);
									mem_manager.charbucket[tag] = NULL;
									mem_manager.charref[tag] = 0;
									mem_manager.charhash[tag] = 0;
									mem_manager.char_address[tag] = 0;
									mem_manager.cs_amount -= 1;
									if(tag == mem_manager.cs_num -1) mem_manager.cs_num -=1;
								}
								else mem_manager.charref[tag] -= 1;
							}
							#ifdef DEBUG
							dbprint(MEMLIBC,"Checking for errors on release",0);
							//list_cs_mem();
							#endif
							return 0;
				case FLOAT: 
							#ifdef DEBUG
							dbprint(MEMLIBC,"Releasing Float Memory with tag", 1, INT, tag);
							//list_f_mem();
							#endif	
							if((tag > mem_manager.f_num  && tag < MAX_MEM) || tag < 0) return -1;
							if(mem_manager.floatbucket[tag] != NULL){
								if(mem_manager.floatref[tag] == 1){ 
									free(mem_manager.floatbucket[tag]);
									mem_manager.floatbucket[tag] = NULL;
									mem_manager.floatref[tag] = 0;
									mem_manager.floathash[tag] = 0;
									mem_manager.float_address[tag] = 0;
									mem_manager.f_amount -= 1;
									if(tag == mem_manager.f_num -1) mem_manager.f_num -=1;
								}
								else if(mem_manager.floatref[tag] > 1){
									mem_manager.floatref[tag] -= 1;
								}
								else{
									;
								}	
							}
							#ifdef DEBUG
							dbprint(MEMLIBC,"Checking for errors on release",0);
							//list_f_mem();
							#endif
							return 0;

				case GENERIC:
							#ifdef DEBUG
							dbprint(MEMLIBC,"Releasing Generic Memory with tag", 1, INT, tag);
							//list_cs_mem();
							#endif	
							if((tag > mem_manager.g_num && tag < MAX_MEM) || tag < 0) return -1;
							if(mem_manager.genericbucket[tag] != NULL){
								if(mem_manager.genericref[tag] == 1){ 
									free(mem_manager.genericbucket[tag]);
									mem_manager.genericbucket[tag] = NULL;
									mem_manager.genericref[tag] = 0;
									mem_manager.generichash[tag] = 0;
									mem_manager.generic_size[tag] = 0;
									mem_manager.generic_address[tag] = 0;
									mem_manager.g_amount -= 1;
									if(tag == mem_manager.g_num -1) mem_manager.g_num -=1;
									}
									else mem_manager.genericref[tag] -= 1;
									}
									#ifdef DEBUG
									dbprint(MEMLIBC,"Checking for errors on release",0);
									//list_cs_mem();
									#endif
									return 0;


				default:
							#ifdef DEBUG
							dbprint(MEMLIBC,"WRONG TYPE",0);
							#endif
							return -1;
			}
		
	}
	else{
		#ifdef DEBUG
		dbprint(MEMLIBC,"Already Released",0);
		#endif
		return 0;
	}
}
int releaseobj(void * obj, typecg intype){
//	int tag;
	int otag;
	int_m * temp;
	float_m * tempfloat;
	str_m * tempstr;
	temp = NULL;
	tempfloat = NULL;
	tempstr = NULL;
	
	if(obj != NULL){
		switch(intype){
			case INT:
						temp = (int_m*) obj;
					//	tag = temp->tag;
						otag = temp->o_tag;
#ifdef DEBUG
						dbprint(MEMLIBC,"found tag ", 1, INT, tag);
						dbprint(MEMLIBC,"found obj tag ", 1, INT, otag);
#endif
						if(mem_manager.objref[otag] > 0 && &mem_manager.objbucket[otag] != NULL){
							if(mem_manager.objref[otag] == 1){ 
								#ifdef DEBUG
								dbprint(MEMLIBC,"Releasing Integer Object ",0);
								#endif
								mem_manager.objref[otag] = 0;
								mem_manager.objhash[otag] = 0;
								mem_manager.obj_address[otag] = 0;
								mem_manager.o_amount -= 1;
								release(temp->value, intype, temp->tag);
								mem_manager.objbucket[otag]=NULL;
								temp->value = NULL;
								temp->o_tag = -1;
								temp->address = 0;
								free(temp);
								temp = NULL;
							}
							else if(mem_manager.objref[otag] > 1){
								#ifdef DEBUG
								dbprint(MEMLIBC,"Reducing Reference Count from ",1, INT, mem_manager.objref[otag]);
								#endif
								 mem_manager.objref[otag] -= 1;
							}
							 else{
								#ifdef DEBUG
								 dbprint(MEMLIBC,"ERROR NEGTIVE REFERENCES",0);
								#endif
								 return -1;
								 ;
							}
						}
						#ifdef DEBUG
						dbprint(MEMLIBC,"Already Released",0);
						#endif
						return 0;
			case FLOAT:
						tempfloat = (float_m*) obj;
					//	tag = tempfloat->tag;
						otag = tempfloat->o_tag;
						if(mem_manager.objref[otag] > 0 && &mem_manager.objbucket[otag] != NULL){
							if(mem_manager.objref[otag] == 1){ 
								#ifdef DEBUG
								dbprint(MEMLIBC,"Releasing Float Object ",0);
								#endif
								mem_manager.objref[otag] = 0;
								mem_manager.objhash[otag] = 0;
								mem_manager.obj_address[otag] = 0;
								mem_manager.o_amount -= 1;
								release(tempfloat->value, intype, tempfloat->tag);
								mem_manager.objbucket[otag]=NULL;
								tempfloat->value = NULL;
								tempfloat->o_tag = -1;
								tempfloat->address = 0;
								free(tempfloat);
								tempfloat = NULL;
							}
							else if(mem_manager.objref[otag] > 1){
								#ifdef DEBUG
								dbprint(MEMLIBC,"Reducing Reference Count from ",1,INT,mem_manager.objref[otag]);
								#endif
								 mem_manager.objref[otag] -= 1;
							}
							 else{
								#ifdef DEBUG
								 dbprint(MEMLIBC,"ERROR NEGTIVE REFERENCES",0);
								#endif
								 return -1;
								 ;
							}
						}
						#ifdef DEBUG
						dbprint(MEMLIBC,"Already Released",0);
						#endif
						return 0;
			
			case STR:
			case CHAR:
						tempstr = (str_m*) obj;
					//	tag = tempstr->tag;
						otag = tempstr->o_tag;
						if(mem_manager.objref[otag] > 0 && &mem_manager.objbucket[otag] != NULL){
							if(mem_manager.objref[otag] == 1){ 
								#ifdef DEBUG
								dbprint(MEMLIBC,"Releasing String/Character Object ",0);
								#endif
								mem_manager.objref[otag] = 0;
								mem_manager.objhash[otag] = 0;
								mem_manager.obj_address[otag] = 0;
								mem_manager.o_amount -= 1;
								release(tempstr->value, intype, tempstr->tag);
								mem_manager.objbucket[otag]=NULL;
								tempstr->value = NULL;
								tempstr->o_tag = -1;
								tempstr->address = 0;
								free(tempstr);
								tempstr = NULL;
							}
							else if(mem_manager.objref[otag] > 1){
								#ifdef DEBUG
								dbprint(MEMLIBC,"Reducing Reference Count from ",1,INT, mem_manager.objref[otag]);
								#endif
								 mem_manager.objref[otag] -= 1;
							}
							 else{
								#ifdef DEBUG
								 dbprint(MEMLIBC,"ERROR NEGTIVE REFERENCES",0);
								#endif
								 return -1;
								 ;
							}
						}
						#ifdef DEBUG
						dbprint(MEMLIBC,"Already Released",0);
						#endif
						return 0;

			default:
						#ifdef DEBUG
						dbprint(MEMLIBC,"WRONG TYPE",0);
						#endif
						return -1;
		}
		
	}
	else{
		#ifdef DEBUG
		dbprint(MEMLIBC,"Already Released",0);
		#endif
		return 0;
	}
}

int releaseallobj(){
	int range;
	#ifdef DEBUG
	dbprint(MEMLIBC,"Releasing All Memory Objects : listing all memory before release", 0);
	dbprint(MEMLIBC,"How many Objects through the o_num variable",1, INT,  mem_manager.o_num);
	#endif	
	if(mem_manager.o_num < MAX_MEM) range = MAX_MEM;
	else range = mem_manager.o_num;
	for(int i = 0; i < range;i++){
		#ifdef DEBUG
		dbprint(MEMLIBC,"Checking object hash to see if empty or not",1, INT, mem_manager.objhash[i]);
		#endif
		if(mem_manager.objhash[i] == 1){
			releaseobj(&mem_manager.objbucket[i],mem_manager.otype[i]);
			mem_manager.objbucket[i] = NULL;
			mem_manager.objhash[i] = 0;
			mem_manager.objref[i] = 0;
			mem_manager.obj_address[i] = 0;
			mem_manager.o_amount -= 1;
			mem_manager.otype[i] = OBJ;
		}
		else{
			#ifdef DEBUG
			dbprint(MEMLIBC,"object bucket fall through",0);
			#endif
			;
		}
	}
	mem_manager.o_amount = 0;
	mem_manager.o_num = 0;
#ifdef DEBUG
	dbprint(MEMLIBC,"Release All Memory Checking for errors",0);
	list_mem(INT,FALSE);
	list_mem(FLOAT,FALSE);
	list_mem(STR,FALSE);
	list_mem(OBJ,FALSE);
#endif
		
	return 0;
}


void *copy_obj(void * obj, typecg intype){
	int tag;
	int_m * temp;
	int_m * iholder;
	float_m * tempfloat;
	float_m * fholder;
	str_m * tempstr;
	str_m * sholder;
	temp = NULL;
	tempfloat = NULL;
	tempstr = NULL;
	iholder = NULL;
	fholder = NULL;
	sholder = NULL;
	
	if(obj != NULL){
		if( (tag = get_avail_mem_tag(OBJ)) != -1){	
			switch(intype){
				case INT:
							iholder = (int_m*)obj;
							if((temp = (int_m*) requestmemobj(1,intype)) ==NULL) return NULL;
							*temp->value = *iholder->value;
							temp->address = iholder->address;
							temp->tag = iholder->tag;
							temp->type = iholder->type;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Tag Given: ",1,  INT, temp->tag);
							dbprint(MEMLIBC,"Value Given:", 1, INT, (void*) temp->value);
							dbprint(MEMLIBC,"Address Equals:", 1, LONG, (void*) temp->address);
							#endif
							return (void*)temp;
				case FLOAT:	
							fholder = (float_m*)obj;
							if((tempfloat = (float_m*) requestmemobj(1,intype)) ==NULL) return NULL;
							*tempfloat->value = *fholder->value;
							tempfloat->address = fholder->address;
							tempfloat->tag = fholder->tag;
							tempfloat->type = fholder->type;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Tag Given: ",1,  INT,  tempfloat->tag);
							dbprint(MEMLIBC,"Value Given:", 1, FLOAT, (void*) tempfloat->value);
							dbprint(MEMLIBC,"Address Equals:", 1, LONG, (void*) tempfloat->address);
							#endif
							return (void*)tempfloat;
							
				case STR:
				case CHAR:
							sholder = (str_m*)obj;
							if((tempstr = (str_m*) requestmemobj(1,intype)) ==NULL) return NULL;
							*tempstr->value = *sholder->value;
							tempstr->address = sholder->address;
							tempstr->tag = sholder->tag;
							tempstr->type = sholder->type;
							#ifdef DEBUG
							dbprint(MEMLIBC,"Tag Given: ", 1, INT,  tempstr->tag);
							dbprint(MEMLIBC,"Value Given:",1, STR, (void*) tempstr->value);
							dbprint(MEMLIBC,"Address Equals:", 1, LONG, (void*) tempstr->address);
							#endif
							return (void*)tempstr;
							
				default:
							return NULL;
			}
		}
	}
	return NULL;	
}
void *assign_to_obj(void* obj1, typecg intype, void* obj2, typecg intype2){
	int_m * temp;
	float_m * tempfloat;
	str_m * tempstr;
	temp = NULL;
	tempfloat = NULL;
	tempstr = NULL;
	
	if(obj1 != NULL){
		switch(intype){
			case INT:
						return 0;
			case FLOAT:
						return 0;
			
			case STR:
			case CHAR:
						return 0;

			default:
						return 0;
		}
		
	}
	return 0;	
}

int mem_cleanup(){
	#ifdef DEBUG
	dbprint(MEMLIBC,"Running Memory Manager Cleanup",0);
	list_mem(STR,FALSE);
	list_mem(INT,FALSE);
	list_mem(FLOAT,FALSE);
	list_mem(GENERIC,FALSE);
//	list_mem(OBJ,FALSE);
	#endif

//	releaseallobj();
	releaseall();
	#ifdef DEBUG
	dbprint(MEMLIBC,"Done.",STR,"");
	dbprint(MEMLIBC,"Checking for Errors", 0);
	list_mem(STR,FALSE);
	list_mem(INT,FALSE);
	list_mem(FLOAT,FALSE);
	list_mem(GENERIC,FALSE);
	#endif
	return 0;
}

void list_i_mem(void ){
	dbprint(MEMLIBC,"Integer Memory ", 1, INT,  mem_manager.i_amount);
	for(int i=0;i<mem_manager.i_num;i++){
		if(mem_manager.inthash[i] == 1){
			dbprint(MEMLIBC,"Allocated in slot", 1, INT, i);
			dbprint(MEMLIBC,"Integer value", 1, INT, *(mem_manager.intbucket[i]));
			dbprint(MEMLIBC,"Number of References", 1, INT, (mem_manager.intref[i]));
			dbprint(MEMLIBC,"Address Given", 1, LONG, (void*) &(mem_manager.intbucket[i]));
		}
	}
}
void list_f_mem(void ){
	dbprint(MEMLIBC,"Float Memory ", 1, INT,  mem_manager.f_amount);
	for(int i=0;i<mem_manager.f_num;i++){
		if(mem_manager.floathash[i] == 1){
			 dbprint(MEMLIBC,"Allocated in slot", 1, INT, i);
			dbprint(MEMLIBC,"Float value", 1, FLOAT, (void*) mem_manager.floatbucket[i]);
 			dbprint(MEMLIBC,"Number of References", 1, INT, mem_manager.floatref[i]);
			dbprint(MEMLIBC,"Address Given", 1, LONG, (void*) &(mem_manager.floatbucket[i]));
		 }
	}
}

void list_g_mem(void ){
	dbprint(MEMLIBC,"Generic Memory Object Memory",1,  INT,  mem_manager.g_amount);
	for(int i=0;i<mem_manager.g_num;i++){
		if(mem_manager.generichash[i] == 1){ 
			dbprint(MEMLIBC,"Allocated in slot", 1, INT, i);
			dbprint(MEMLIBC,"Size in Bytes",1,  INT, mem_manager.generic_size[i]);
//			dbprint(MEMLIBC,"String/Character value", STR, mem_manager.charbucket[i]);
			dbprint(MEMLIBC,"Number of References",1,  INT, mem_manager.genericref[i]);
			dbprint(MEMLIBC,"Address Given",1,  LONG, (void*) &(mem_manager.genericbucket[i]));
		}
	}
}


void list_cs_mem(void){
	dbprint(MEMLIBC,"String/Character Memory ",1,  INT,  mem_manager.cs_amount);
	for(int i=0;i<mem_manager.cs_num;i++){
		if(mem_manager.charhash[i] == 1){ 
			dbprint(MEMLIBC,"Allocated in slot",1,  INT, i);
			dbprint(MEMLIBC,"String/Character value",1,  STR, mem_manager.charbucket[i]);
			dbprint(MEMLIBC,"Number of References",1,  INT, mem_manager.charref[i]);
			dbprint(MEMLIBC,"Address Given",1,  LONG, (void*) &(mem_manager.charbucket[i]));
		}
	}
}
void list_o_mem(boolcg SUPPRESS_EMPTY_HASH){
	int ftag;
	int_m *temp;
	float_m *tempfloat;
	str_m * tempstr;
	temp = NULL;
	tempfloat = NULL;
	tempstr = NULL;
	dbprint(MEMLIBC,"-- Number of Objects Allocated --",1,  INT,  mem_manager.o_amount);
	for(int i=0;i<MAX_MEM;i++){
		temp = NULL;
		tempfloat=NULL;
		tempstr=NULL;
		if(mem_manager.objhash[i] == 1){ 
			dbprint(MEMLIBC,"-  Object Allocated in slot  -",1,  INT, i);
			switch(mem_manager.otype[i]){
				case INT:
							temp = (int_m*)mem_manager.objbucket[i];
							if(mem_manager.inthash[temp->tag] == 1){
								dbprint(MEMLIBC,"- Integer Allocated for Object in slot", 1, INT, temp->tag);
								dbprint(MEMLIBC,"-- Integer value", 1, INT, *(mem_manager.intbucket[temp->tag]));
								dbprint(MEMLIBC,"-- Number of References", 1, INT , (mem_manager.intref[temp->tag]));
								dbprint(MEMLIBC,"-- Integer Address Given", 1, LONG, (void*)& (mem_manager.intbucket[temp->tag]));
							}
							break;
				case FLOAT:
						tempfloat = (float_m*)mem_manager.objbucket[i];
						ftag = tempfloat->tag;
						if(mem_manager.floathash[ftag] == 1){
							 dbprint(MEMLIBC,"- Float Allocated for Object in slot", 1, INT, (ftag));
						 	dbprint(MEMLIBC,"-- Float value", 1, FLOAT, (void*)  mem_manager.floatbucket[ftag]);
		 					 dbprint(MEMLIBC,"-- Number of References", 1, INT, mem_manager.floatref[ftag]);
							 dbprint(MEMLIBC,"-- Float Address Given",1,  LONG, (void*) &(mem_manager.floatbucket[ftag]));
						 }
						 break;
				case CHAR:
				case STR:
						tempstr = (str_m*)&mem_manager.objbucket[i];
						if(mem_manager.charhash[tempstr->tag] == 1){ 
							dbprint(MEMLIBC,"- String/Character Allocated for Object in slot",1,  INT, tempstr->tag);
							dbprint(MEMLIBC,"-- String/Character value", 1, STR, mem_manager.charbucket[tempstr->tag]);
							dbprint(MEMLIBC,"-- Number of References",1,  INT, mem_manager.charref[tempstr->tag]);
							dbprint(MEMLIBC,"-- String/Character Address Given", 1, LONG, (void*) &(mem_manager.charbucket[tempstr->tag]));
						}
						break;
				case OBJ:
				default: break;
			}
//			debugprint(2,"String/Character value", mem_manager.objbucket[i]);
			dbprint(MEMLIBC,"-Object Number of References",1,  INT, mem_manager.objref[i]);
			dbprint(MEMLIBC,"-Object Address Given",1,  LONG, (void*) &(mem_manager.objbucket[i]));
		}
		else{
			if(!SUPPRESS_EMPTY_HASH) dbprint(MEMLIBC,"Object Hash is empty ",1, INT, i);
		}
	}
}
void list_mem(typecg intype, boolcg SUPPRESS_EMPTY_HASH){
	switch(intype){
		case INT: list_i_mem();break;
		case FLOAT: list_f_mem();break;
		case STR:
		case CHAR: list_cs_mem();break;
		case GENERIC: list_g_mem();break;
		case OBJ:	list_o_mem(SUPPRESS_EMPTY_HASH);break;
		default:	break;
	}
}
