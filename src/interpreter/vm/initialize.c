#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "initialize.h"

typecg *used_type1;
typecg *used_type2;
typecg *used_type3;


void initialize_machine(void){

    used_type1 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type1 = INT;
    used_type2 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type2 = FLOAT;
    used_type3 = requestmem(sizeof(typecg),GENERIC,&tag);
    *used_type3 = STR;
    
    init_memstack(&vm_memstack);

}
