/*
 * error.c
 *
 *  Created on: 15. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "error.h"

void warning_msg(const char *fmt, ...)
{
	va_list listPointer;
	va_start(listPointer, fmt);
	vfprintf(stderr,fmt, listPointer);
	va_end(listPointer);
}

void error_msg(const char *fmt, ...)
{
	va_list listPointer;
	va_start(listPointer, fmt);
	vfprintf(stderr,fmt, listPointer);
	va_end(listPointer);
	exit(1);
}
