/**
 * process_working.h
 *
 *  Created on: 18. 4. 2017
 *    Solution: IOS-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 *
**/

#ifndef PROCESS_WORKING_H_
#define PROCESS_WORKING_H_
#include <stdio.h>
#include <sys/types.h>
void help();
void adult();
void set_resources();
void clean_resources();
void child();
void load_args(int argc, char **argv);
void error_output(int chyba);
void delay_rutine(int maxtime);
void print_state(char * processName,int counter, char* state);
void print_waiting(char * processName,int counter, int a, int c);
#endif /*PROCESS_WORKING_H_*/
