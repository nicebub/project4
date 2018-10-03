#include "vm.h"

#include "../memlib.h"
#include "../debuglib.h"

int main(int argc, const char* argv[]){
	initialize_machine();
	init_mem_man();
//	showstack();
	push(INT, (int[]) { 35 } );
	push(STR, (char[]) { "me,myself,irene" } );
	push(INT, (int[]) { 67 } );
	push(STR, (char[]) { "me,myself,irene4" } );
	push(STR, (char[]) { "me,myself,irene3" } );
	push(STR, (char[]) { "me,myself,irene2" } );
	push(INT, (int[]) { 42 } );
	push(FLOAT,(float[]) { 3.14159 } );
	push(FLOAT,(float[]) { 8.675309 } );
	push(FLOAT,(float[]) { 1009.42 } );
	push(STR, (char[]) { "EAT AT JOES" } );
	push(STR, (char[]) { "well now how is this string doing" } );
	push(INT, (int[]) { 444 } );
	push(INT, (int[]) { 555 } );
	push(INT, (int[]) { 111 } );
	push(FLOAT,(float[]) { 0.0000001 } );
	push(FLOAT,(float[]) { 0.045 } );
	push(FLOAT,(float[]) { 1.7 } );
//	showstack();
	typecg intype;
	void *firstint = pop(&intype, 5);
	SWITCH(intype, dbprint(0,"--POPPED OFF--",1, INT, *(int*) firstint), \
		dbprint(0,"-POPPED OFF-",1, FLOAT,*(float*)firstint), \
		dbprint(0,"-POPPED OFF-",1, STR,(void*)firstint), \
		dbprint(0,"-POPPED OFF-",1, LONG,*(unsigned long*)firstint))
//	showstack();
	push(STR, (char[]) { "Minsitry of silly walks" } );
	push(INT, (int[]) { 401398337 } );
	unsigned long mylogg= 44444442222;
	push(LONG , (void*)&mylogg );
//	showstack();
	push(LONG , ( unsigned long[]) { 22424 } );
//	showstack();
	push(FLOAT, (float[]) { 346.89 } );
//	showstack();
	while(vm_memstack.stacksize!=3){
		pop(&intype, 3);
	}

	push(STR, (char[]) { "who can read this" } );
	push(STR, (char[]) { "must ask themselves" } );
	push(STR, (char[]) { "are they worthy?" } );
	push(LONG, (unsigned long[]) { 549442 } );
	push(LONG, (unsigned long[]) { 108888 } );

//	showstack();
//	void * address_bar = vm_stack.top[0];
	push(LONG, (unsigned long* [] ) { firstint } );
//	push(LONG, (unsigned long* [] ) { vm_memstack.top[0] } );
//	showstack();
	mem_cleanup();
	return 0;
}
