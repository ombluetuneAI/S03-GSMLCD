#ifndef COMMON_H
#define COMMON_H

#include "stdbool.h"
#include "sys.h"

#define  OS_ENTER_CRITICAL()  {__disable_irq() ;}			/* disable interrupt*/
#define  OS_EXIT_CRITICAL()   {__enable_irq();}			/* enable interrupt*/





#ifndef ShiftLeft
  #define ShiftLeft(n)      (1 << (n))
#endif

#ifndef ShiftRight
  #define ShiftRight(n)      (1 >> (n))
#endif

#ifndef BF
  #define BF(x,b,s)  (((x) & (b)) >> (s))
#endif

#ifndef MIN
  #define MIN(n,m)   (((n) < (m)) ? (n) : (m))
#endif

#ifndef MAX
  #define MAX(n,m)   (((n) < (m)) ? (m) : (n))
#endif

#ifndef ABS
  #define ABS(n)     (((n) < 0) ? -(n) : (n))
#endif



#define st(x)      do { x } while (__LINE__ == -1)
  

#endif

/*-------------------------end------------------*/







































