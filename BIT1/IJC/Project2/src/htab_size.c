/*
 * htab_size.c
 *
 *  Created on: 4. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
size_t htab_size(htab_t *htab)
{
	if(htab == NULL)
		return 0;
	return htab->htab_act_size;
}
