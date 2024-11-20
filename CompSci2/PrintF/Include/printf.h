#include <stdarg.h>
#include <stdio.h>

#ifndef __PRINTF_H_
#define __PRINTF_H_

//Define the __USING_C_ macro only if we don't have it.
#ifndef __USING_C_
#define __USING_C_ extern "C"
#endif//__USING_C_

__USING_C_ void cprintf(const char* format, ...);
__USING_C_ int StrToInteger(const char* str);

#undef __USING_C_
#endif//__PRINTF_H_
