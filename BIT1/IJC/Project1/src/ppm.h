/*
 * ppm.h
 *
 *  Created on: 15. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#ifndef PPM_H_
#define PPM_H_
#include <stdio.h>

struct ppm {
	unsigned xsize;
	unsigned ysize;
	char data[];
};

struct ppm * ppm_read(const char * filename);
int ppm_read_head(FILE *file, int *height, int *width);
int ppm_write(struct ppm *p, const char * filename);
#endif /* PPM_H_ */
