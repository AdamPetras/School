/*
 * steg-decode.h
 *
 *  Created on: 20. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#ifndef STEG_DECODE_H_
#define STEG_DECODE_H_
#include "ppm.h"
#include "eratosthenes.h"
const char *load_arg(int argc,char **argv);
void load_message(struct ppm *data);

#endif /* STEG_DECODE_H_ */
