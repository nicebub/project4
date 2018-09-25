#include "newmemory.h"

#define TEST(temp,value,realtype,type) set_cell_value(&temp, &(realtype){value}, type, (size_t)1); \
		  	print_memory_cell(&temp);

#define printer     printf("printting stack\n"); \
		  	print_stack(&mstack); \
			printf("stack size : %d\n", (int)mstack.stacksize); \
			printf("the base pointer is now: what cell\n"); \
			print_memory_cell(mstack.bp); \
			printf("just under the stack pointer is now: what cell\n"); \
			print_memory_cell(mstack.sp-1); \


#define setup_record(a,m,c,n,i,r)     inrecord.access_link = a; \
			inrecord.alloc_amount = m; \
		  	inrecord.control_link = c; \
			strcpy(inrecord.last_command_name,n); \
			inrecord.last_command_instruction = i; \
			strcpy(inrecord.returnvalue,r); \


memstack vm_memstack;
static memstack mstack;

int main(void){
    static memory_cell temp;
    
    init_memstack(&mstack);
    init_memory_cell(&temp);
    push_onto_stack(&mstack,&(int){32}, INT);
    push_onto_stack(&mstack,"hi",STR);
    push_onto_stack(&mstack,&(double){5.4},FLOAT);
    void * mval = &(int[]){1,5,3,2};
    push_onto_stack(&mstack,&mval,REFINT);
    push_onto_stack(&mstack,mval,REFINT);
    push_onto_stack(&mstack,*(int*)mval,REFINT);
    printf("the memory address for mval is: %p\n", &mval);
    printf("the memory address for mval is: %p\n", mval);
    printf("the memory address for mval is: %d\n", *(int*)mval);
    printer
    pop_off_stack(&mstack);
    printer
    change_stack_value(&mstack,1, "hello", STR);
    printer
    activationrecord inrecord;
    setup_record("link",5,"control","mycommand",11,"25");
    push_activation_record(&mstack, inrecord);
    printer
    push_onto_stack(&mstack,inrecord.control_link,STR);
    push_onto_stack(&mstack,inrecord.access_link,STR);
    printer
    setup_record("newlink",10,"newcontrol","nextcommand",4,"my_new_");
    push_activation_record(&mstack, inrecord);
    printer
    printf("value at offset 0: %s\n", get_value_at_offset(&mstack,0));
    printf("value at offset 1: %s\n", get_value_at_offset(&mstack,1));
    printf("value at offset 2: %s\n", get_value_at_offset(&mstack,2));
    printf("value at offset 3: %s\n", get_value_at_offset(&mstack,3));
    printf("value at offset 4: %d\n", *(int*)get_value_at_offset(&mstack,4));
    printf("value at offset 5: %d\n", *(int*)get_value_at_offset(&mstack,5));
    printf("value at offset -1: %s\n", get_value_at_offset(&mstack,-1));
    printf("value at offset -2: %s\n", get_value_at_offset(&mstack,-2));
    printf("value at offset 1 frame back: %s\n",
		 get_value_at_offset_n_frames_back(&mstack,0,1));
    printf("value at offset 2 frames back: %d\n",
		 *(int*)get_value_at_offset_n_frames_back(&mstack,0,2));
    printf("value at offset 0 frames back: %s\n",
		 get_value_at_offset_n_frames_back(&mstack,0,0));
    printf("value at offset 2 frames back: %p\n",
		 get_value_at_offset_n_frames_back(&mstack,3,2));
    printf("value at offset 1 frame back: %s\n",
		 get_value_at_offset_n_frames_back(&mstack,0,1));
    printer
    pop_activation_record(&mstack);
    printer
    pop_activation_record(&mstack);
    printer
//    delete_stack(&mstack);
	return 0;
}
