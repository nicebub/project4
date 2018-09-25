#include "newmemory.h"





void init_memory_cell(memory_cell *incell){
	   incell->m.size = 0;
	   incell->m.type = GENERIC;
	   incell->m.s.address = NULL;
	   incell->m.s.string = NULL;
	   incell->m.s.character = '\0';
	   incell->m.s.num = -1;
	   incell->m.s.real = -1.1;
}

void reinit_memory_cell(memory_cell *incell){
   if(incell->m.type == STR)
	  free(incell->m.s.string);
   init_memory_cell(incell);
}


inline void set_cell_size(memory_cell *incell, size_t s){
	   incell->m.size = s;
}
inline size_t get_cell_size(memory_cell *incell){
	   return incell->m.size;
}

inline void set_cell_type(memory_cell *incell,typecg t){
	   incell->m.type = t;
}

inline typecg get_cell_type(memory_cell *incell){
	   return incell->m.type;
}


void set_cell_value(memory_cell *incell,void * v, typecg t, size_t s){
    if(incell->m.type == STR){
	   reinit_memory_cell(incell);
    }
	   switch(t){
		  case INT:
			 incell->m.s.num = *(int*)v;
			 break;
		  case FLOAT:
			 incell->m.s.real = *(double*)v;
			 break;
		  case CHAR:
			 incell->m.s.character = *(char*)v;
			 break;
		  case STR:
			  incell->m.s.string = strdup((char*)v);
			  break;
		  default:
			 incell->m.s.address = v;
			 break;
	   }
	   set_cell_size(incell, s);
	   set_cell_type(incell,t);
}

void* get_cell_value(memory_cell *incell){
    switch(incell->m.type){
	   case INT:
		  return &incell->m.s.num;
	   case FLOAT:
		  return &incell->m.s.real;
	   case CHAR:
		  return &incell->m.s.character;
	   case STR:
		  return incell->m.s.string;
	   default:
		  return incell->m.s.address;
    }
    return NULL;
}

void print_memory_cell(memory_cell *incell){
 //   char *a;
    printf("Memory Cell Contents: ");
    switch(incell->m.type){
	   case INT:
		  printf("%d\n", incell->m.s.num);
		  printf("Memory Cell Address : %p\n", &incell->m.s.num);
		  break;
	   case FLOAT:
		  printf("%f\n", incell->m.s.real);
		  printf("Memory Cell Address : %p\n", &incell->m.s.real);
		  break;
	   case CHAR:
	   		 printf("%c\n", incell->m.s.character);
	  	   	 printf("Memory Cell Address : %p\n", &incell->m.s.character);
	  	   	break;
	   case STR:
		   printf("%s\n", incell->m.s.string);
		   printf("Memory Cell Address : %p\n", &incell->m.s.string);
		   break;
	   default:
		  printf("%p\n", incell->m.s.address);
		  printf("Memory Cell Address : %p\n", &incell->m.s.address);
		  break;
    }

}

void init_memstack(memstack *instack){
	   for(int q=0;q<STACK_SIZE;q++)
		  	init_memory_cell(&instack->stack[q]);
	   instack->bp = &instack->stack[0];
	   instack->sp = &instack->stack[0];
	   instack->stacksize = 0;
}


void* pop_off_stack(memstack *instack){
	void * value;
	if(instack->sp->m.type == STR){
		value = strdup((char*)get_cell_value(instack->sp-1));
	}
	else{
	    value = get_cell_value(instack->sp-1);
	}
	reinit_memory_cell(instack->sp-1);
	instack->stacksize--;
	instack->sp--;
    return value;
}

void push_onto_stack(memstack *instack, void * value, typecg intype){
		  set_cell_value(instack->sp,value,intype,1);
		  instack->stacksize++;
		  instack->sp++;
}

void change_stack_value(memstack *instack, int offset, void* value, typecg intype){
    set_cell_value(&instack->stack[offset], value, intype, 1);
}


void print_stack(memstack *instack){
    for(int j=0;j<instack->stacksize;j++){
	   printf("Memory Cell Full Offset: %d\n", j);
	   print_memory_cell(&instack->stack[j]);
    }
}

void delete_stack(memstack *instack){
	for(int j=0;j<instack->stacksize;j++)
		if(instack->stack[j].m.type == STR){
		    free(instack->stack[j].m.s.string);
				instack->stack[j].m.s.string = NULL;
		}
}

boolcg stack_isempty(memstack *instack){
	if(instack->stacksize == 0)
		return TRUE;
    return FALSE;
}

size_t get_stack_size(memstack *instack){
	return instack->stacksize;
}

/*struct _activationrecord {
    char	last_command_name[MAX_STR_CONST];
    char	returnvalue[MAX_SIZE];
    char	* control_link;
    char	* access_link;
    int		last_command_instruction;
    int		alloc_amount;
};*/

void push_activation_record(memstack *instack, activationrecord inrecord){
    memory_cell *temp = instack->sp;
    push_onto_stack(instack,inrecord.last_command_name,STR);
    push_onto_stack(instack,inrecord.returnvalue,STR);
    push_onto_stack(instack,instack->bp,REFSTR);
    push_onto_stack(instack,inrecord.access_link,REFSTR);
    push_onto_stack(instack,&inrecord.last_command_instruction,INT);
    push_onto_stack(instack,&inrecord.alloc_amount,INT);
    instack->bp = temp;
}
void * pop_activation_record(memstack *instack){
  //  activationrecord *temp;
//    memory_cell * tempcell;
    memory_cell newbase;
    init_memory_cell(&newbase);
    set_cell_value(&newbase,(instack->bp+2)->m.s.address,get_cell_type((instack->bp+2)),get_cell_size((instack->bp+2)));
    memory_cell *tempcell = instack->bp;
    while(tempcell != instack->sp)
	   pop_off_stack(instack);
    instack->bp = newbase.m.s.address;
    return NULL;
}

void * get_value_at_offset(memstack *instack,int offset){
    return get_cell_value((instack->bp+offset));
}
void * get_value_at_offset_n_frames_back(memstack *instack,int offset,int n){
    memory_cell *temp;
    temp = instack->bp;;
    for(int i=n;i>0;i--){
	   temp = (temp+2)->m.s.address;
    }
    return get_cell_value(temp+offset);
}
