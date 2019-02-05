/*
 * htab_foreach.c
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
#include <stdio.h>
void htab_foreach(htab_t *htab,  void (*callback)(char key[], unsigned value))
{
	if(htab!=NULL||htab_size(htab)==0)
	for (unsigned int i =0;i<htab_bucket_count(htab);i++)
		//for cyklus pro průchod celým polem položek
	{
		for(htab_listitem *tmp = htab->list_item[i];tmp != NULL;tmp=tmp->next)
			//for cyklus pro průchod všemi položkami
		{
			callback(tmp->key,tmp->data);
		}
	}
}
void print_values(char key[], unsigned value)
{
	printf("%s %i\n",key,value);
}
