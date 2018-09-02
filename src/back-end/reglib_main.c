#include "reglib.h"
#include "memlib.h"
#include "debuglib.h"
#include <stdio.h>
//#include <stdlib.h>


int main(int argc, char** argv){
	reg *firstreg[16];
		for(int i=0;i<16;i++)
			firstreg[i] = NULL;

	init_mem_man();

	initialize_register_stack();
	print_register_stack();

	//delete_register_stack();

		fprintf(stdout,"registers_left() %d\n", registers_left());

		for(int i=15; i >=0;i--){
			firstreg[i] = get_next_register();
			fprintf(stdout,"registers_left() %d\n", registers_left());
			print_register_stack();
		}

			release_register(firstreg[0]);
			firstreg[0]= NULL;
			fprintf(stdout,"registers_left() %d\n", registers_left());
			print_register_stack();
			release_register(firstreg[1]);
			firstreg[1] = NULL;
			fprintf(stdout,"registers_left() %d\n", registers_left());
			print_register_stack();
			release_register(firstreg[2]);
			firstreg[2] = NULL;
			fprintf(stdout,"registers_left() %d\n", registers_left());
			print_register_stack();
			release_register(firstreg[7]);
			firstreg[7] = NULL;
			fprintf(stdout,"registers_left() %d\n", registers_left());
			print_register_stack();
			firstreg[0] = get_next_register();
			fprintf(stdout,"registers_left() %d\n", registers_left());
			print_register_stack();
			firstreg[3] = get_next_register();
			fprintf(stdout,"registers_left() %d\n", registers_left());
			print_register_stack();
			release_register(firstreg[15]);
			firstreg[15] = NULL;
//			fprintf(stdout,"registers_left() %d\n", registers_left());
			fprintf(stdout,"registers_left() %d\n", registers_left());
			fprintf(stdout,"LAST REGISTER STACK PRINT\n");
			print_register_stack();
	fprintf(stdout,"registers_left() %d\n", registers_left());


	mem_cleanup();
	return 0;
}