/*
 * htab_free.c
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
void htab_free(htab_t *htab)
{
	if(htab == NULL)
		return;
	htab_clear(htab);
	free(htab);
}
