/*
 * htab_clear.c
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
void htab_clear(htab_t *htab)
{
	//cyklus for pro projdení celé tabulky
	for(unsigned int i =0;i<htab_bucket_count(htab);i++)
	{
		//for cyklus pro průchod všemi položkami
		if(htab->list_item[i] != NULL)
		{
			for(htab_listitem *item = htab->list_item[i]; item != NULL;item = item->next )
			{
				//printf("removed");
				htab_remove(htab,item->key);
			}
		}
	}
}
