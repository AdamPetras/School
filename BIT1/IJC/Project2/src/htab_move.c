/*
 * htab_move.c
 *
 *  Created on: 4. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
#include <string.h>
htab_t *htab_move(int new_size,htab_t *htab)
{
	htab_t *new_table = htab_init(new_size);
	if(new_table != NULL)
	{
		for(unsigned int i =0;i<htab_bucket_count(htab);i++)
		{
			for (htab_listitem *old_table_item = htab->list_item[i];old_table_item!= NULL;old_table_item=old_table_item->next)
			{
				unsigned int index = hash_function(old_table_item->key)%new_size;	//zavolání hashovací fce
				htab_listitem *tmp;
				htab_listitem *last;
				if(new_table->list_item[index] == NULL)		//pokud ještě neni na indexu nic
				{
					new_table->list_item[index]= create_item_to_list(old_table_item->key,NULL);		//vytvoření nové položky
					new_table->list_item[index]->data = index;
					if(new_table->list_item[index]==NULL)		//pokud neni alokována
					return NULL;
				}
				else	//pokud již je index obsazen
				{
					for(tmp = new_table->list_item[index];tmp != NULL;tmp=tmp->next)	//prolezení seznamu až nakonec
					{
						last = tmp;
						continue;
					}
					tmp = create_item_to_list(old_table_item->key,last);	//vytvoření nové položky
					tmp->data = index;
					if(tmp == NULL)		//pokud neni alokována
						return NULL;
				}
				new_table->htab_act_size++;		//inkrementace a dekrementace velikosti
				htab->htab_act_size--;
				free(old_table_item);	//odstranění položky ze staré tabulky
			}
			htab->list_item[i] = NULL;
		}
		new_table->htab_size = new_size;
		return new_table;
	}
	return NULL;
}
