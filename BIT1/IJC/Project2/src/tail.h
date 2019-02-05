/*
 * tail.h
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#ifndef TAIL_H_
#define TAIL_H_
#define SIZE 1024
void buffer_clean(char * buffer, size_t size);
char **dyn_std(unsigned int *num_lines, size_t size);
int tail_read(int num_line,int line_count,FILE * pFile,int isprint);
#endif /* TAIL_H_ */
