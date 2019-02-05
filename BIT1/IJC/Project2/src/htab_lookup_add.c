/*
 * htab_lookup_add.c
 *
 *  Created on: 4. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
#include <stdlib.h>
#include <string.h>
htab_listitem *create_item_to_list(const char *key, htab_listitem *item_before)
{
	htab_listitem *tmp;
	int number_of_chars = strlen(key)+1;	//zjištění délky klíče pro alokaci + ukončovací "\0"
	size_t size = sizeof(char)*number_of_chars;	//výpočet delky
	tmp = malloc(sizeof(htab_listitem)+size);	//alokace paměti
	if(tmp != NULL)	//pokud se povedla
	{
		memcpy(tmp->key,key,number_of_chars);	//kopírování položky z key do tmp->key
		if(item_before != NULL) //pokud neni item_before null tak předání pointeru
			item_before->next = tmp;
		tmp->next = NULL;	//next je null
		tmp->data=0;	//nastavení dat
		return tmp;
	}
	return NULL;
}

htab_listitem *htab_lookup_add(htab_t *htab, const char *key)
{
	unsigned int index = hash_function(key)%(htab->htab_size);	//zjištění indexu z hash fce
	if(htab_find(htab,key)!= NULL)
	{
		return htab_find(htab,key);	//pokud je nalezen vracím
	}
	else
	{
		if(htab->list_item[index]!= NULL)	//pokud ukazatel již ukazuje na seznam(položku)
		{
			htab_listitem* tmp;
			htab_listitem* last;
			for(tmp = htab->list_item[index];tmp != NULL;tmp=tmp->next)	//projití seznamu nakonec
			{
				last = tmp;
				continue;
			}
			tmp = create_item_to_list(key,last);	//vytvoření položky
			htab->htab_act_size++;	//zvýšení aktuální velikosti
			if(tmp != NULL)	//pokud je vytvořen vracím
			{
				return tmp;
			}
			return NULL;
		}
		else
		{	//jinak vytvořím položku která odkazuje na base pointer tabulky
		htab->list_item[index] = create_item_to_list(key,NULL);
		htab->htab_act_size++;
		if(htab->list_item[index] != NULL)	//pokud je vytvořen vracím
			return htab->list_item[index];
		else
			return NULL;
		}

	}
	return NULL;
}

