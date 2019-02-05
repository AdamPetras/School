/*
 * htab_find.c
 *
 *  Created on: 4. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
#include <string.h>
htab_listitem *htab_find(htab_t *htab,const char *key)
{
	unsigned int index = (hash_function(key)%htab->htab_size);
	for(htab_listitem* tmp = htab->list_item[index];tmp != NULL;tmp=tmp->next)
	{
		//smyčka pro vyhledávání
		if(strcmp(tmp->key,key) == 0)
		{
			return tmp;
		}
	}
	return NULL;
}

