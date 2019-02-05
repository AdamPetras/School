/*
 * htab_init.c
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#include "htable.h"

unsigned int hash_function(const char *str)
{
	unsigned int h=0;
	const unsigned char *p;
	for(p=(const unsigned char*)str; *p!='\0'; p++)
		h = 65599*h + *p;
	return h;
}

htab_t *htab_init(unsigned size)
{
	size_t item_size = size*sizeof(htab_listitem);	//velikost položek
	htab_t *htab = malloc(sizeof(htab_t)+item_size);	//alokace velikosti structu + velikosti položek
	if (htab != NULL)	//pokud je naalokován
	{
		for (int i =0;i<size;i++)	//inicializace všech listů
			htab->list_item[i]=NULL;
		htab->htab_size = size;	//přiřazení maximální velikosti
		htab->htab_act_size = 0; 	//přiřazení aktuální velikosti
		return htab;
	}
	return NULL;
}

