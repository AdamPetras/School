/*
 * htab_remove.c
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
#include <string.h>
Bool htab_remove(htab_t *htab, const char *key)
{
		unsigned index = hash_function(key)%htab_bucket_count(htab);	//získání indexu
		htab_listitem* tmp;
		htab_listitem* last = NULL;
		for(tmp = htab->list_item[index];tmp != NULL;)	//cyklus na prolezení celého seznamu
		{
			if(strcmp(tmp->key,key) == 0)	//porovnání klíčů
			{
				if(tmp == htab->list_item[index])	//pokud klíč je na první položce
					htab->list_item[index] = tmp->next;	//nastavení basepointeru na pointer další
				else
					last->next = tmp->next;	//"přepointerování"
				htab->htab_act_size--;	//odečtění act velikosti
				last = tmp->next;	//poslední pointer ukazuje na další pointer toho aktuálního
				free(tmp);	//uvolnění
				tmp = last;
				return true;
			}
			last = tmp;	//last je nyní aktuální protože jdeme do další iterace
		}
	return false;	//vrací false
}

