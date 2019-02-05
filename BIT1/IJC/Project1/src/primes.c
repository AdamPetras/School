/*
 ============================================================================
 Name        : primes.c
 Solution    : IJC-DU1, příklad
 Author      : Adam Petráš, FIT
 Compiled    : GCC 5.4.0 20160609
 Copyright   : Your copyright notice
 Description : Eratosthenovo síto
 ============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "eratosthenes.h"
int main(int argc, char **argv) {
	if(argc==2)
	{
		if(strcmp(argv[1],"-h") == 0)
		{
			printf("This is program to calculate primes via Eratosthenes.\nYou have to set in linux \"ulimit -s unlimited\".\n\nCreated by Adam Petráš\nVUT FIT BIT1.\n");
			exit(0);
		}
		else
		{
			static int counter = 0;
			ba_create(array, 303000000);
			Eratosthenes(array);
			unsigned long tmp[10];
			for (unsigned long i =303000000-1;i>1;i--)
			{
				if(ba_get_bit(array,i)==0)
				{
					tmp[counter]=i;
					counter++;
					if(counter==10)
					{
						break;
					}
				}
			}
			for(int i =9;i>=0;i--)
			{
				printf("%lu\n",tmp[i]);
			}
		}
	}
	return EXIT_SUCCESS;
}
