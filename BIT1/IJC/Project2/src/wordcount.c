/*
 * wordcount.c
 *
 *  Created on: 10. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#include "htable.h"
#include "io.h"

int main(void)
{
	//zkoušeno na knize TWENTY YEARS AFTER kterou vydal Alexandre Dumas při hodnotě 10 utilita time ukazovala asi 1.21sec při hodnotě 3000 hodnota time ukazovala 0.100sec a při hodnotě 6000-40000 ukazuje asi 0.0910 sec 
	//je to zřejmé, že většina slov je rozčleněna do více buňěk hash než aby byla v seznamu který má složitost asymptotickou a díky v podstatě konstantní složitosti hash table přístup tak rychlý
	
	htab_t* htab = htab_init(6000);
	char word[128];
	while(get_word(word, 127,stdin) != EOF)
	{
		htab_lookup_add(htab,word)->data++;	//najdení slova nebo přidání jestli neexistuje a inkrementace dat
	}
	htab_foreach(htab, print_values);	//výpis
	htab_free(htab);	//uvolnění paměti
	return 0;
}
