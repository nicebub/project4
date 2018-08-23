#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif
#include "memlib.h"
#include "debuglib.h"
#include "Listcg.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

extern mem_man mem_manager;

void init_mem_man(){
	#ifdef DEBUG
	dbprint(2,"Initializing Memory Manager",STR, (void*)"");
/*	list_mem(INT);
	list_mem(FLOAT);
	list_mem(STR);*/
	list_mem(OBJ);
	#endif
	
	mem_manager.i_num = 0;
	mem_manager.f_num = 0;
	mem_manager.cs_num = 0;
	mem_manager.o_num = 0;
	mem_manager.f_amount = 0;
	mem_manager.cs_amount = 0;
	mem_manager.i_amount = 0;
	mem_manager.o_amount = 0;
	
	for(int i = 0; i < MAX_MEM;i++){
		mem_manager.intbucket[i] = NULL;
		mem_manager.charbucket[i] = NULL;
		mem_manager.floatbucket[i] = NULL;
		mem_manager.objbucket[i] = NULL;
		mem_manager.otype[i] = OBJ;
	}
	#ifdef DEBUG
	dbprint(2,"Done.",STR,"");
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
					
		default: 
					return -1;
	}
}

void * requestmem(int size, typecg intype, int *tag){
	int * temp;
	char * tempchar;
	float * tempfloat;
//	#ifdef DEBUG
//	dbprint(2,"Requesting Memory",STR,"");
//	#endif
	if( (*tag = get_avail_mem_tag(intype)) != -1){	
		switch(intype){
			case INT:
						#ifdef DEBUG
						dbprint(2,"Requesting Memory for an Integer",STR,"");
						#endif
						temp = (int* )malloc(sizeof(int)*size);
						mem_manager.intbucket[*tag] = temp;
						mem_manager.inthash[*tag] = 1;
						mem_manager.intref[*tag] += 1;
						mem_manager.int_address[*tag] = (unsigned long) temp;
						mem_manager.i_amount += 1;
						#ifdef DEBUG
						dbprint(2,"Integer Address after creation",LONG,(void*)&mem_manager.int_address[*tag]);
						#endif
						if(mem_manager.i_num < MAX_MEM) ++(mem_manager.i_num);
						return (void*)temp;
			case CHAR:
			case STR:
						#ifdef DEBUG
						dbprint(2,"Requesting Memory for a String/Character",STR,"");
						#endif
						tempchar = (char*)malloc(sizeof(char)*size);
						mem_manager.charbucket[*tag] = tempchar;
						for(int i = 0; i< sizeof(char)*size;i++)
							tempchar[i]='\0';
						mem_manager.charhash[*tag] = 1;
						mem_manager.charref[*tag] += 1;
						mem_manager.cs_amount += 1;
						mem_manager.char_address[*tag] = (unsigned long) tempchar;
						#ifdef DEBUG
						dbprint(2,"String Address after creation",LONG,(void*)&mem_manager.char_address[*tag]);
						#endif
						if(mem_manager.cs_num < MAX_MEM) ++(mem_manager.cs_num);
						return (void*)tempchar;
			case FLOAT: 
						#ifdef DEBUG
						dbprint(2,"Requesting Memory for a Float",STR,"");
						#endif
						tempfloat = (float*)malloc(sizeof(float)*size);
						mem_manager.floatbucket[*tag] = tempfloat;
						mem_manager.floathash[*tag] = 1;
						mem_manager.floatref[*tag] += 1;
						mem_manager.f_amount += 1;
						mem_manager.float_address[*tag] = (unsigned long) tempfloat;
						#ifdef DEBUG
						dbprint(2,"Float Address after creation",LONG,(void*)&mem_manager.float_address[*tag]);
						#endif
						if(mem_manager.f_num < MAX_MEM) ++(mem_manager.f_num);
						return (void*)tempfloat;
			default:
						return NULL;
		}
	}
	else{
		#ifdef DEBUG
		dbprint(2,"Couldn't get a memory unit tag",INT, (void*)&tag);
		#endif
		return NULL;
	}
}

void * requestmemobj(int size, typecg intype){
	int tag;
	int_m * temp;
	float_m * tempfloat;
	str_m * tempstr;
	temp = NULL;
	tempfloat = NULL;
	tempstr = NULL;
//	#ifdef DEBUG
//	dbprint(2,"Requesting Memory for an Object",STR,"");
//	#endif

	if( (tag = get_avail_mem_tag(OBJ)) != -1){	
	
		switch(intype){
			case INT: 
						#ifdef DEBUG
						dbprint(2,"Requesting Memory for an Integer Object",STR,"");
						#endif
						temp = (int_m* )malloc(sizeof(int_m));
						temp->value = requestmem(size, intype, &(temp->tag));
						if( temp->tag != -1){
							temp->type = INT;
							temp->address = (unsigned long)temp->value;
							mem_manager.objhash[tag]  = 1;
							mem_manager.obj_address[tag]  = (unsigned long) temp;
							mem_manager.objbucket[tag]  = temp;
//							#ifdef DEBUG
//							dbprint(2,"Object Bucket Tag", INT, &tag);
//							dbprint(2,"Object Bucket", STR, mem_manager.objbucket[tag]);
//							#endif
							mem_manager.objref[tag] += 1;
							mem_manager.o_amount += 1;
							#ifdef DEBUG
							dbprint(2,"Amount of allocated Objects", INT, &mem_manager.o_amount);
							#endif
							mem_manager.otype[tag] = INT;
							if(mem_manager.o_num < MAX_MEM) ++(mem_manager.o_num);
							#ifdef DEBUG
							dbprint(2,"Object Tag: ", INT, (void*)&tag);
							//dbprint(2,"Object Starting Value:", INT, (void*) temp->value);
							dbprint(2,"Object Address:", LONG, (void*)&mem_manager.obj_address[tag]);
							dbprint(2,"Integer Tag: ", INT, (void*)&temp->tag);
							dbprint(2,"Integer Starting Value:", INT, (void*) temp->value);
							dbprint(2,"Integer Address:", LONG, (void*)&mem_manager.int_address[temp->tag]);
							dbprint(2,"",STR,"");
							#endif
							return (void*)temp;
						}
						free(temp);
						temp = NULL;
						return NULL;
			case FLOAT:
						#ifdef DEBUG
						dbprint(2,"Requesting Memory for a Float Object",STR,"");
						#endif
						tempfloat = (float_m* )malloc(sizeof(float_m));
						tempfloat->value = requestmem(size, intype, &(tempfloat->tag));
						if( tempfloat->tag != -1){
							tempfloat->type = FLOAT;
							tempfloat->address = (unsigned long)tempfloat->value;
							
							mem_manager.objhash[tag]  = 1;
							mem_manager.obj_address[tag]  = (unsigned long) tempfloat;
							mem_manager.objbucket[tag]  = temp;
							mem_manager.objref[tag] += 1;
							mem_manager.o_amount += 1;
							#ifdef DEBUG
							dbprint(2,"Amount of allocated Objects", INT, &mem_manager.o_amount);
							#endif
							mem_manager.otype[tag] = FLOAT;
							if(mem_manager.o_num < MAX_MEM) ++(mem_manager.o_num);
							#ifdef DEBUG
							dbprint(2,"Object Tag: ", INT, (void*)&tag);
							//dbprint(2,"Object Starting Value:", INT, (void*) temp->value);
							dbprint(2,"Object Address:", LONG, (void*)&mem_manager.obj_address[tag]);
							dbprint(2,"Float Tag: ", INT, (void*)&tempfloat->tag);
							dbprint(2,"FLoat Starting Value:", FLOAT, (void*) tempfloat->value);
							dbprint(2,"Float Address:", LONG, (void*)&mem_manager.float_address[tempfloat->tag]);
							dbprint(2,"",STR,"");
							#endif
							return (void*)tempfloat;
						}	
						free(tempfloat);
						tempfloat=NULL;
						return NULL;
		
			case STR:
			case CHAR:
						#ifdef DEBUG
						dbprint(2,"Requesting Memory for a String/Character Object",STR,"");
						#endif
						tempstr = (str_m* )malloc(sizeof(str_m));
						tempstr->value = requestmem(size, intype, &(tempstr->tag));
						if( tempstr->tag != -1){
							temp->type = STR;
							tempstr->address = (unsigned long) tempstr->value;
							mem_manager.objhash[tag]  = 1;
							mem_manager.obj_address[tag]  = (unsigned long) temp;
							mem_manager.objbucket[tag]  = temp;
							mem_manager.objref[tag] += 1;
							mem_manager.o_amount += 1;
							mem_manager.otype[tag] = STR;
							if(mem_manager.o_num < MAX_MEM) ++(mem_manager.o_num);
							#ifdef DEBUG
							dbprint(2,"Object Tag: ", INT, (void*)&tag);
							//dbprint(2,"Object Starting Value:", INT, (void*) temp->value);
							dbprint(2,"Object Address:", LONG, (void*)&mem_manager.obj_address[tag]);
							dbprint(2,"String/Character Tag: ", INT, (void*)&tempstr->tag);
							dbprint(2,"String/Character Starting Value:", STR, (void*) tempstr->value);
							dbprint(2,"String/Character Address:", LONG, (void*)&mem_manager.char_address[tempstr->tag]);
							dbprint(2,"",STR,"");
							#endif
							return (void*)tempstr;
						}
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
	debugprint(2,"Releasing All Memory"," : listing all memory before release");
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
	}
	mem_manager.i_amount = 0;
	mem_manager.i_num = 0;
	for(int i = 0; i < mem_manager.f_num;i++){
		if(mem_manager.floatbucket[i] != NULL) free(mem_manager.floatbucket[i]);
		mem_manager.floatbucket[i] = NULL;
		mem_manager.floathash[i] = 0;
		mem_manager.floatref[i] = 0;
		mem_manager.float_address[i] = 0;
	}
	mem_manager.f_amount = 0;
	mem_manager.f_num = 0;
	for(int i = 0; i < mem_manager.cs_num;i++){
		if(mem_manager.charbucket[i] != NULL) free(mem_manager.charbucket[i]);
		mem_manager.charbucket[i] = NULL;
		mem_manager.charhash[i] = 0;
		mem_manager.charref[i] = 0;
		mem_manager.char_address[i] = 0;
	}
	mem_manager.cs_amount = 0;
	mem_manager.cs_num = 0;
#ifdef DEBUG
	debugprint(2,"Release All Memory Checking for errors","");
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
	if(mem !=NULL){
		if(tag > MAX_MEM){
				#ifdef DEBUG
				dbprint(2,"tag greater than MAX_MEM", INT, &tag);
				#endif	
				 return -1;
			 }
			switch(intype){
				case INT: 	
							#ifdef DEBUG
							debugprintd(3,"Releasing Integer Memory with tag", tag);
							//list_i_mem();
							#endif	
							if(tag > mem_manager.i_num || tag < 0){
								#ifdef DEBUG
								dbprint(2,"tag greater than i_num or tag < 0", INT, &tag);
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
								}
								else if(mem_manager.intref[tag] > 1){
									#ifdef DEBUG
									dbprint(2,"Reducing Reference Count from ",INT,&mem_manager.intref[tag]);
									#endif
									 mem_manager.intref[tag] -= 1;
								}
								 else{
									 ;
								}
							}
							#ifdef DEBUG
							debugprint(2,"Checking for errors on release","");
							//list_i_mem();
							#endif
							return 0;
				case CHAR:
				case STR:
							#ifdef DEBUG
							debugprintd(3,"Releasing String/Character Memory with tag", tag);
							//list_cs_mem();
							#endif	
							if(tag > mem_manager.cs_num || tag < 0) return -1;
							if(mem_manager.charbucket[tag] != NULL){
								if(mem_manager.charref[tag] == 1){ 
									free(mem_manager.charbucket[tag]);
									mem_manager.charbucket[tag] = NULL;
									mem_manager.charref[tag] = 0;
									mem_manager.charhash[tag] = 0;
									mem_manager.char_address[tag] = 0;
									mem_manager.cs_amount -= 1;
								}
								else mem_manager.charref[tag] -= 1;
							}
							#ifdef DEBUG
							debugprint(2,"Checking for errors on release","");
							//list_cs_mem();
							#endif
							return 0;
				case FLOAT: 
							#ifdef DEBUG
							debugprintd(3,"Releasing Float Memory with tag", tag);
							//list_f_mem();
							#endif	
							if(tag > mem_manager.f_num || tag < 0) return -1;
							if(mem_manager.floatbucket[tag] != NULL){
								if(mem_manager.floatref[tag] == 1){ 
									free(mem_manager.floatbucket[tag]);
									mem_manager.floatbucket[tag] = NULL;
									mem_manager.floatref[tag] = 0;
									mem_manager.floathash[tag] = 0;
									mem_manager.float_address[tag] = 0;
									mem_manager.f_amount -= 1;
								}
								else mem_manager.floatref[tag] -= 1;
							}
							#ifdef DEBUG
							debugprint(2,"Checking for errors on release","");
							//list_f_mem();
							#endif
							return 0;
				default:
							#ifdef DEBUG
							dbprint(2,"WRONG TYPE",STR,"");
							#endif
							return -1;
			}
		
	}
	else{
		#ifdef DEBUG
		dbprint(2,"Already Released",STR,"");
		#endif
		return 0;
	}
}
int releaseobj(void * obj, typecg intype){
	int tag;
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
						tag = temp->tag;
						if(mem_manager.objref[tag] != 0 && &mem_manager.objbucket[tag] != NULL){
							if(mem_manager.objref[tag] == 1){ 
								#ifdef DEBUG
								dbprint(2,"Releasing Integer Object ",STR,"");
								#endif
								mem_manager.objbucket[tag]=NULL;
								mem_manager.objref[tag] = 0;
								mem_manager.objhash[tag] = 0;
								mem_manager.obj_address[tag] = 0;
								mem_manager.o_amount -= 1;
								release(temp->value, intype, temp->tag);
								temp->value = NULL;
								temp->tag = -1;
								temp->address = 0;
								free(temp);
								temp = NULL;
							}
							else if(mem_manager.objref[tag] > 1){
								#ifdef DEBUG
								dbprint(2,"Reducing Reference Count from ",INT,&mem_manager.objref[tag]);
								#endif
								 mem_manager.objref[tag] -= 1;
							}
							 else{
								#ifdef DEBUG
								 dbprint(2,"ERROR NEGTIVE REFERENCES",STR,"");
								#endif
								 return -1;
								 ;
							}
						}
						#ifdef DEBUG
						dbprint(2,"Already Released",STR,"");
						#endif
						return 0;
			case FLOAT:
						tempfloat = (float_m*) obj;
						tag = tempfloat->tag;
						if(mem_manager.objbucket[tag] != NULL){
							if(mem_manager.objref[tag] == 1){ 
								#ifdef DEBUG
								dbprint(2,"Releasing Float Object ",STR,"");
								#endif
								mem_manager.objbucket[tag]=NULL;
								mem_manager.objref[tag] = 0;
								mem_manager.objhash[tag] = 0;
								mem_manager.obj_address[tag] = 0;
								mem_manager.o_amount -= 1;
								release(tempfloat->value, intype, tempfloat->tag);
								tempfloat->value = NULL;
								tempfloat->tag = -1;
								tempfloat->address = 0;
								free(tempfloat);
								tempfloat = NULL;
							}
							else if(mem_manager.objref[tag] > 1){
								#ifdef DEBUG
								dbprint(2,"Reducing Reference Count from ",INT,&mem_manager.objref[tag]);
								#endif
								 mem_manager.objref[tag] -= 1;
							}
							 else{
								#ifdef DEBUG
								 dbprint(2,"ERROR NEGTIVE REFERENCES",STR,"");
								#endif
								 return -1;
								 ;
							}
						}
						#ifdef DEBUG
						dbprint(2,"Already Released",STR,"");
						#endif
						return 0;
			
			case STR:
			case CHAR:
						tempstr = (str_m*) obj;
						tag = tempstr->tag;
						if(mem_manager.objbucket[tag] != NULL){
							if(mem_manager.objref[tag] == 1){ 
								#ifdef DEBUG
								dbprint(2,"Releasing String/Character Object ",STR,"");
								#endif
								mem_manager.objbucket[tag]=NULL;
								mem_manager.objref[tag] = 0;
								mem_manager.objhash[tag] = 0;
								mem_manager.obj_address[tag] = 0;
								mem_manager.o_amount -= 1;
								release(tempstr->value, intype, tempstr->tag);
								tempstr->value = NULL;
								tempstr->tag = -1;
								tempstr->address = 0;
								free(tempstr);
								tempstr = NULL;
							}
							else if(mem_manager.objref[tag] > 1){
								#ifdef DEBUG
								dbprint(2,"Reducing Reference Count from ",INT,&mem_manager.objref[tag]);
								#endif
								 mem_manager.objref[tag] -= 1;
							}
							 else{
								#ifdef DEBUG
								 dbprint(2,"ERROR NEGTIVE REFERENCES",STR,"");
								#endif
								 return -1;
								 ;
							}
						}
						#ifdef DEBUG
						dbprint(2,"Already Released",STR,"");
						#endif
						return 0;

			default:
						#ifdef DEBUG
						dbprint(2,"WRONG TYPE",STR,"");
						#endif
						return -1;
		}
		
	}
	else{
		#ifdef DEBUG
		dbprint(2,"Already Released",STR,"");
		#endif
		return 0;
	}
}

int releaseallobj(){
	#ifdef DEBUG
	debugprint(2,"Releasing All Memory Objects"," : listing all memory before release");
	/*list_mem(INT);
	list_mem(FLOAT);
	list_mem(STR);
	list_mem(OBJ);*/
	dbprint(2,"How many Objects through the o_num variable", INT, (void*)&mem_manager.o_num);
	#endif	
	for(int i = 0; i < mem_manager.o_num;i++){
		#ifdef DEBUG
		dbprint(2,"Checking wither bucket shows empty or not",INT,(void*)&mem_manager.objhash[i]);
		#endif
		if((mem_manager.objbucket[i]) != NULL){
			releaseobj(mem_manager.objbucket[i],mem_manager.otype[i]);
			mem_manager.objbucket[i] = NULL;
			mem_manager.objhash[i] = 0;
			mem_manager.objref[i] = 0;
			mem_manager.obj_address[i] = 0;
			mem_manager.o_amount = 0;
			mem_manager.otype[i] = OBJ;
		}
		else{
			#ifdef DEBUG
			dbprint(2,"object bucket fall through",STR,"");
			#endif
		}
	}
	mem_manager.o_num = 0;
#ifdef DEBUG
	debugprint(2,"Release All Memory Checking for errors","");
	list_mem(INT);
	list_mem(FLOAT);
	list_mem(STR);
	list_mem(OBJ);
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
							dbprint(2,"Tag Given: ", INT, (void*)&temp->tag);
							dbprint(2,"Value Given:", INT, (void*) temp->value);
							dbprint(2,"Address Equals:", LONG, (void*)&temp->address);
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
							dbprint(2,"Tag Given: ", INT, (void*)&tempfloat->tag);
							dbprint(2,"Value Given:", FLOAT, (void*) tempfloat->value);
							dbprint(2,"Address Equals:", LONG, (void*)&tempfloat->address);
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
							dbprint(2,"Tag Given: ", INT, (void*)&tempstr->tag);
							dbprint(2,"Value Given:", STR, (void*) tempstr->value);
							dbprint(2,"Address Equals:", LONG, (void*)&tempstr->address);
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
	dbprint(2,"Running Memory Manager Cleanup",STR, (void*)"");
	/*list_mem(INT);
	list_mem(FLOAT);
	list_mem(STR);*/
	list_mem(OBJ);
	#endif

	releaseallobj();
	releaseall();
	#ifdef DEBUG
	dbprint(2,"Done.",STR,"");
	dbprint(2,"Checking for Errors", STR,"");
	list_mem(INT);
	list_mem(FLOAT);
	list_mem(STR);
	list_mem(OBJ);
	list_mem(OBJ);
	
	#endif
	return 0;
}

#ifdef DEBUG
void list_i_mem(){
	dbprint(2,"Integer Memory ", INT, (void*)&mem_manager.i_amount);
	for(int i=0;i<mem_manager.i_num;i++){
		if(mem_manager.inthash[i] == 1){
			debugprintd(2,"Allocated in slot", i);
			debugprintd(2,"Integer value", *(mem_manager.intbucket[i]));
			debugprintd(2,"Number of References", (mem_manager.intref[i]));
			dbprint(2,"Address Given", LONG, (void*)&(mem_manager.int_address[i]));
		}
	}
}
void list_f_mem(){
	dbprint(2,"Float Memory ", INT, (void*) &mem_manager.f_amount);
	for(int i=0;i<mem_manager.f_num;i++){
		if(mem_manager.floathash[i] == 1){
			 debugprintd(2,"Allocated in slot", i);
			dbprint(2,"Float value", FLOAT, (void*) mem_manager.floatbucket[i]);
 			debugprintd(2,"Number of References", mem_manager.floatref[i]);
			dbprint(2,"Address Given", LONG, (void*)&(mem_manager.float_address[i]));
		 }
	}
}
void list_cs_mem(){
	dbprint(2,"String/Character Memory ", INT, (void*)&mem_manager.cs_amount);
	for(int i=0;i<mem_manager.cs_num;i++){
		if(mem_manager.charhash[i] == 1){ 
			debugprintd(2,"Allocated in slot", i);
			debugprint(2,"String/Character value", mem_manager.charbucket[i]);
			debugprintd(2,"Number of References", mem_manager.charref[i]);
			dbprint(2,"Address Given", LONG, (void*)&(mem_manager.char_address[i]));
		}
	}
}
void list_o_mem(){
	int_m *temp;
	float_m *tempfloat;
	str_m * tempstr;
	dbprint(2,"-- Number of Objects Allocated --", INT, (void*)&mem_manager.o_amount);
	for(int i=0;i<MAX_MEM;i++){
		if(mem_manager.objhash[i] == 1){ 
			debugprintd(2,"-  Object Allocated in slot  -", i);
			switch(mem_manager.otype[i]){
				case INT:
							temp = (int_m*)mem_manager.objbucket[i];
							if(mem_manager.inthash[temp->tag] == 1){
								debugprintd(2,"- Integer Allocated for Object in slot", temp->tag);
								debugprintd(2,"-- Integer value", *(mem_manager.intbucket[temp->tag]));
								debugprintd(2,"-- Number of References", (mem_manager.intref[temp->tag]));
								dbprint(2,"-- Integer Address Given", LONG, (void*)&(mem_manager.int_address[temp->tag]));
							}
							break;
				case FLOAT:
						tempfloat = (float_m*)mem_manager.objbucket[i];
					 	dbprint(2,"----tempfloat->tag", LONG, (void*) &tempfloat->tag);
						if(mem_manager.floathash[tempfloat->tag] == 1){
							 debugprintd(2,"- Float Allocated for Object in slot", tempfloat->tag);
						 	dbprint(2,"-- Float value", FLOAT, (void*) mem_manager.floatbucket[tempfloat->tag]);
		 					 debugprintd(2,"-- Number of References", mem_manager.floatref[tempfloat->tag]);
							 dbprint(2,"-- Float Address Given", LONG, (void*)&(mem_manager.float_address[tempfloat->tag]));
						 }
						 break;
				case CHAR:
				case STR:
						tempstr = (str_m*)mem_manager.objbucket[i];
						if(mem_manager.charhash[tempstr->tag] == 1){ 
							debugprintd(2,"- String/Character Allocated for Object in slot", tempstr->tag);
							debugprint(2,"-- String/Character value", mem_manager.charbucket[tempstr->tag]);
							debugprintd(2,"-- Number of References", mem_manager.charref[tempstr->tag]);
							dbprint(2,"-- String/Character Address Given", LONG, (void*)&(mem_manager.char_address[tempstr->tag]));
						}
						break;
				case OBJ:
				default: break;
			}
//			debugprint(2,"String/Character value", mem_manager.objbucket[i]);
			debugprintd(2,"-Object Number of References", mem_manager.objref[i]);
			dbprint(2,"-Object Address Given", LONG, (void*)&(mem_manager.obj_address[i]));
		}
		else{
			dbprint(2,"Object Hash is empty ",INT,&i);
		}
	}
}
void list_mem(typecg intype){
	switch(intype){
		case INT: list_i_mem();break;
		case FLOAT: list_f_mem();break;
		case STR:
		case CHAR: list_cs_mem();break;
		case OBJ:	list_o_mem();break;
		default:	break;
	}
}
#endif