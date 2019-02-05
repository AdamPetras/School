
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, říjen 2014
**                              Radek Hranický, listopad 2015
**                              Radek Hranický, říjen 2016
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht )
{
	for (int i=1;i<HTSIZE;i++)	//nastavená všechprvních ukazatelů na zřetězená synonyma na NULL
		(*ptrht)[i] = NULL;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key )
{
	//ultra hnusný for ale je to jednoduché... první vyhledání indexu pomocí hash fce poté porovnání a poté už další iterace kdy přenastavím pointer a zkontroluju jestli neni null
	for (tHTItem *tmp = (*ptrht)[hashCode(key)];tmp != NULL;tmp =tmp->ptrnext)
		if(strcmp(tmp->key,key)==0)	//pokud je kláč roven key
			return tmp;	//nalezeno vrátím
	return NULL; 	//nenalezeno vratím NULL
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data )
{
	tHTItem *tmp;		//deklarace pomocné tmp
	if((tmp = htSearch(ptrht, key)) != NULL)		//pokud je již tmp(klíč) vtabulce
		tmp->data = data;	//pouze aktualizuju data
	else	//pokud není
	{
		tHTItem *temp;
		tmp = (*ptrht)[hashCode(key)];	//tmp je ukazatel na položku v tabulce vyhledaný pomocí hash fce
		if((temp = malloc(sizeof(tHTItem))) == NULL)	//alokace paměti a oštření
		{
			return;		//nepovedla se alokace
		}
		temp->data = data;	//temp data jsou data
		temp->ptrnext = tmp;	//temp next ptr je tmp
		if((temp->key = malloc(strlen(key)+1)) == NULL)		//alokace paměti pro klíč
		{
			return;		//nepovedla se alokace
		}
		memcpy(temp->key,key,strlen(key)+1);	//kopírování klíče do temp klíče pomocí mem copy
		(*ptrht)[hashCode(key)] = temp;		//nastavení proměnné do tabulky
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key )
{
	if(htSearch(ptrht,key) == NULL)		//pokud nenajdu vracím NULL
		return NULL;
	return &(htSearch(ptrht,key)->data);	//vracím ukazatel na data
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key )
{
	if((*ptrht)[hashCode(key)] == NULL)		//pokud je ukazatel v tabulce na indexu který nám vyhodila hash fce NULL
		return;
	tHTItem *delete,*tmp;
	//tmp je nastavený na položku v tabulce danou hash fcí pokud další ukazatel na tom stejném míště kde
	//ukázala hash není NULL a klíč si neni rovén tak nastavujeme tmp na next pointer
	for(tmp = (*ptrht)[hashCode(key)];tmp->ptrnext != NULL && strcmp(tmp->key,key) !=0; tmp = tmp->ptrnext)
		delete = tmp;	//vložení do pomocné ho ukazatelu
	if(strcmp(tmp->key, key)== 0)		//pokud je tmp key a key rovno
	{
		if(tmp->ptrnext == NULL)	//a pokud tmp další ukazatel je NULL
			delete->ptrnext = NULL;		//nastavení delete next ukazatele na NULL
		else if(delete == tmp)		//jinak pokud jsou si ukazatele delete a tmp rovyn
		{
			if(tmp->ptrnext == NULL)		//a pokud je next ukazatel NULL
				(*ptrht)[hashCode(key)] = NULL;		//v tabulce danou hash fcí nastavíme ukazatel na NULL
			else (*ptrht)[hashCode(key)] = tmp->ptrnext;		//jinak nastavníme ukazatel na ten dalsí (jakoby o jeden)
		}
		else if(tmp->ptrnext != NULL && delete != tmp)	// a pokud tmp next ukazatel neni NULL a delete neni tmp
		{
			delete->ptrnext = tmp->ptrnext;	// nastavení delete next ukazatele na tmp next ukazatele
		}
		free(tmp->key);	//uvolnění klíče
		free(tmp);		//uvolnění položky
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht )
{
	for(int i =1;i<HTSIZE;i++)		//pro všechny políčka v hash tabulce
	{
		//for cyklus pro projítí celého zřetězeního seznamu na každé položce hash tabulky
		for (tHTItem *tmp = (*ptrht)[i];tmp != NULL;tmp = tmp->ptrnext)
		{
			tHTItem *temp = tmp;	//temp je tmp
			free(temp->key);		//uvolnění klíče
			free(temp);		//uvolnění ukazatele
		}
		(*ptrht)[i] = NULL;		//nastavení tabulky na NULL
	}
}
