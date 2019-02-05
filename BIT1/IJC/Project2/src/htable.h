/*
 * htable.h
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#ifndef HTABLE_H_
#define HTABLE_H_
#include <stdlib.h>
#include "Bool.h"
typedef struct  htab_listitem_s {
	unsigned int data;
	struct htab_listitem_s *next;
	char key[];
} htab_listitem;

/**
 * hash tabulka
 */
typedef struct  htab {
	unsigned htab_size;
	unsigned htab_act_size;
	struct htab_listitem_s *list_item[];
} htab_t;

void print_values(char key[], unsigned value);
/**
 * vytvoření a alokování záznamu listu
 */
htab_listitem *create_item_to_list(const char *key, htab_listitem *item_before);
/**
 * hashovací funkce
 */
unsigned int hash_function(const char *str);
/**
 * přesypání jedné tabulky do druhé s novou velikostí
 */
htab_t *htab_move(int new_size,htab_t *t);
/**
 * konstruktor tabulky
 */
htab_t *htab_init(unsigned size);
/**
 * vrátí počet prvků tabulky
 */
size_t htab_size(htab_t *t);
/**
 * vrátí počet všech prvků v poli
 */
size_t htab_bucket_count(htab_t *t);
/**
 * vyhledávání prvku v tabulce
 */
htab_listitem *htab_lookup_add(htab_t *t, const char *key);
/**
 * vyhledávání prvku v tabulce
 */
htab_listitem *htab_find(htab_t *t,const char *key);
/**
 * volání fukce nějaké funkce pro každý prvek tabulky
 */
void htab_foreach(htab_t *t,  void (*callback)(char key[], unsigned value));
/**
 * odstranění položky
 */
Bool htab_remove(htab_t *t, const char *key);
/**
 * odstranění kompletně celé tabulky
 */
void htab_clear(htab_t *t);
/**
 * odstranění kompletně celé tabulky a uvolnění paměti
 */
void htab_free(htab_t *t);

#endif /* HTABLE_H_ */
