#ifndef _REGLIB_H
#define _REGLIB_H
#include "memlib.h"

#ifdef DEBUGON
#ifndef DEBUG
#define DEBUG
#endif
#endif

#define REGISTER64_COUNT 16
#define REGISTER32_COUNT 8
#define REGISTER16_COUNT 8
#define REGISTER8_COUNT 8

typedef enum {
	REG1,
	REG2,
	REG3,
	REG4,
	REG5,
	REG6,
	REG7,
	REG8,
	REG9,
	REG10,
	REG11,
	REG12,
	REG13,
	REG14,
	REG15,
	REG16
} bit64_bitreg;


static const char *bit64_strings[] = {
	"rax", "rbx", "rcx", "rdx", "rbp", "rsi", "rdi", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
};

typedef enum {
	REG32_1,
	REG32_2,
	REG32_3,
	REG32_4,
	REG32_5,
	REG32_6,
	REG32_7,
	REG32_8

} bit32_bitreg;


static const char *bit32_strings[] = {
	"eax", "ebx", "ecx", "edx", "ebp", "esi", "edi", "esp"
};

typedef enum {
	REG16_1,
	REG16_2,
	REG16_3,
	REG16_4,
	REG16_5,
	REG16_6,
	REG16_7,
	REG16_8

} bit16_bitreg;


static const char *bit16_strings[] = {
	"ax", "bx", "cx", "dx", "bp", "si", "di", "sp"
};

typedef enum {
	REG8L_1,
	REG8L_2,
	REG8L_3,
	REG8L_4,
	REG8L_5,
	REG8L_6,
	REG8L_7,
	REG8L_8

} lower_8_bitreg;


static const char *bit8_lower_strings[] = {
	"al", "bl", "cl", "dl", "bp", "si", "di", "sp"
};

typedef enum {
	REG8H_1,
	REG8H_2,
	REG8H_3,
	REG8H_4,
	REG8H_5,
	REG8H_6,
	REG8H_7,
	REG8H_8

} higher_8_bitreg;


static const char *bit8_higher_strings[] = {
	"ah", "bh", "ch", "dh", "bp", "si", "di", "sp"
};

typedef struct _reg reg;
struct _reg {
	bit64_bitreg reg_type;
	reg * next_register;
};

typedef struct _register_stack register_stack;
struct _register_stack {
	reg * top;
	int stacksize;
	int tag[REGISTER64_COUNT];
	reg *inthewild[REGISTER64_COUNT];
};

extern register_stack Register_Stack;

void initialize_register_stack(void);
void delete_register_stack(void);
reg* get_next_register(void);
void release_register(reg* inRegister);
int registers_left(void);
#ifdef DEBUG
void print_register_stack(void);
#endif

#endif
