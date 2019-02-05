/*
 * eratosthenes.c
 *
 *  Created on: 12. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include <math.h>
#include "eratosthenes.h"
void Eratosthenes(bit_array_t array)
{
	unsigned long tmp = 0;
	double m = sqrt(ba_size(array));
	ba_set_bit(array,0,1);
	ba_set_bit(array,1,1);
	for (unsigned long i = 2;i<m;i++)
	{
		if(ba_get_bit(array,i) == 0)
		{
			for(tmp=i*i;tmp<=ba_size(array)-1;tmp+=i)
			{
				if(ba_get_bit(array,tmp) == 0)
				{
					//printf("%i = %i\n",tmp,1);
					ba_set_bit(array,tmp,1);
				}
			}
		}
	}
}
