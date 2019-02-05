/*
 * htab_bucked_count.c
 *
 *  Created on: 4. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "htable.h"
size_t htab_bucket_count(htab_t *htab)
{
	return htab->htab_size;
}
