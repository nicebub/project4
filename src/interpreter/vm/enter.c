#if defined(DEBUGON) && defined(VMLIBDEBUG) && !defined(DEBUG)
#define DEBUG
#endif

#if defined(DEBUGON) && !defined(VMLIBDEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#if !defined(DEBUGON) && defined(DEBUG)
#undef DEBUG
#endif

#include "enter.h"

void enter(int inscope){
	#ifdef DEBUG
    dbprint(VMLIBC,"FOUND AN ENTER IN MACHINE",0);
	#endif

#ifdef DEBUG
	#endif

}
