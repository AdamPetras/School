/*
 * error.h
 *
 *  Created on: 15. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#ifndef ERROR_H_
#define ERROR_H_
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
void warning_msg(const char *fmt, ...);
void error_msg(const char *fmt, ...);



#endif /* ERROR_H_ */
