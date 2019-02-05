
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**                                         Úpravy: Radek Hranický, říjen 2016
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů 
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**	
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.	
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/
	*RootPtr = NULL;	//inicializace nastavení rootu na NULLptr
	
}	

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/
	if(RootPtr == NULL)		//pokud je root null return false (nenalezeno)
		return FALSE;
	if(RootPtr->Key == K)	//pokud se root rovná K
	{
		*(Content) = RootPtr->BSTNodeCont;	//předání ukazatele na prvek přes content
		return TRUE;	//vrátím nalezeno
	}
	if(RootPtr->RPtr != NULL)	//pokud není pravý ukazatel NULL
		if(RootPtr->Key < K )	// a pokud je K větší než klíč kořene(rootu)
			return BSTSearch(RootPtr->RPtr,K,Content);		//rekurzivní volání fce (dokud nenarazí na...)
	if(RootPtr->LPtr != NULL)	//pokud není pravý ukazatel NULL
		if(RootPtr->Key > K)	//pokud je K menší než klíč kořene(rootu)
			return BSTSearch(RootPtr->LPtr,K,Content);		//rekurzivní volání fce (dokud nenarazí na...)
	return FALSE;	//nenalezeno vrací false
} 


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{	
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
	tBSTNodePtr root = *RootPtr;	//pomocná proměnná je kořen stromu(dereference ukazatele)
	if(root->Key == K)	//pokud je Key kořene rovno K
	{
		root->BSTNodeCont = Content;	//nastavím nodeCont na Content
		return;		//skáču z fce
	}
	if(root->Key < K)	//pokud je K větší než Key
	{
		if(root->RPtr == NULL)	//pokud je root RPtr NULL
		{
			tBSTNodePtr tmp;
			if((tmp = malloc(sizeof(struct tBSTNode))) == NULL)	//alokace pomocné proměnné a její oštření
			{
				return;//špatná alokace
			}
			root->RPtr = tmp;	//nastavení pravého ukazatele na tmp
			root->RPtr->RPtr = NULL;	//nastavení jeho pravého a levého ukazatele na NULL
			root->RPtr->LPtr = NULL;
			root->RPtr->BSTNodeCont = Content;		//nastavení hodnoty na Content
			root->RPtr->Key = K; 		//nastavení kliče
			return;
		}
		BSTInsert(&root->RPtr,K,Content);	//rekurzivní voolání fce pro insert (jelikož RPtr nebyl NULL)
		return;
	}
	if(root->Key > K)	//pokud je K menší než klíč
	{
		if(root->LPtr == NULL)		//pokud je LPtr NULL
		{
			tBSTNodePtr tmp;
			if((tmp = malloc(sizeof(struct tBSTNode))) == NULL)	//alokace pomocné proměnné a její oštření
			{
				return;//špatná alokace
			}
			root->LPtr = tmp;	//nastavení pravého ukazatele na tmp
			root->LPtr->RPtr = NULL;	//nastavení jeho pravého a levého ukazatele na NULL
			root->LPtr->LPtr = NULL;
			root->LPtr->BSTNodeCont = Content;		//nastavení hodnoty na Content
			root->LPtr->Key = K;		//nastavení kliče
			return;
		}
		BSTInsert(&root->LPtr,K,Content);	//rekurzivní voolání fce pro insert (jelikož LPtr nebyl NULL)
		return;
	}
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně. 
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete(). 
**/
	tBSTNodePtr root = *RootPtr;	//nastavení rootu na kořen stromu(jeho dereferenci ukazatele)
	if(root->RPtr == NULL)	//pokud je RPtr NULL
	{
		PtrReplaced->Key = root->Key;	//nastavení klíče změněného na klíč kořene
		PtrReplaced->BSTNodeCont = root->BSTNodeCont;		//nastavení node cont na hodnotu kořene node cont
	}
	else
		ReplaceByRightmost(PtrReplaced, &root->RPtr);		//rekurzivní volání fce protože RPtr nebyl NULL
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic. 
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
	if(*RootPtr == NULL)	//pokud je kořen NULL
		return;		//return NULL
	tBSTNodePtr root = *RootPtr;		//nastavení rootu na kořen stromu(jeho dereferenci ukazatele)
	if(root->Key != K)	//pokud se key kořene nerovná K (pokračujeme k synům)
	{
		if(root->RPtr != NULL)	//pokud se RPtr nerovná NULL
		{
			if(root->Key < K)	//a K je větší jak Key
			{
				if(root ->RPtr-> Key == K)		// a K je rovno root->RPtr klíči
				{
					if(root->RPtr->RPtr == NULL)	//a také root RPtr RPtr je NULL
						if(root ->RPtr->LPtr == NULL)	// a root RPtr LPtr je NULL
						{
							 BSTDelete(&(root)->RPtr,K);	//rekurzivní volání funkce a předání pravého syna
							 root->RPtr = NULL;		//pravý ukazatel kořene nastavíme na NULL
						}
				}
				else	//pokud K neni rovno root->RPtr klíči
				{
					BSTDelete(&(root)->RPtr,K);		//rekurzivní volání funkce a předání pravého syna
				}
			}
		}
		else if(root ->LPtr != NULL)	 //pokud LPtr neni NULL
		{
			if(root->Key >K)		// K je menší než kořenové Key
			{
				if(root ->LPtr-> Key == K)	// pokud root LPtr klíč je roven K
				{
					if(root->LPtr->RPtr == NULL)	//pokud root LPtr RPtr je rovno NULL
						if(root ->LPtr->LPtr == NULL)	//a rooot LPtr LPtr je rovno NULL
						{
							 BSTDelete(&(root)->LPtr,K);	//rekurzivní volání funkce a předání levého syna
							 root->LPtr = NULL;		//nastavení ukazatele na levý prvek kořene na NULL
						}
				}
				else
				{
					BSTDelete(&(root)->LPtr,K);		//rekurzivní volání funkce a předání levého syna
				}
			}
		}
	}
	else if (root->Key == K)	//jinak pokud je K rovno root Key
	{
		if(root->RPtr == NULL)	//a pokud je pravý ukazatel NULL
		{
			if(root->LPtr != NULL)	// ale levý null není
			{
				tBSTNodePtr node = root->LPtr;	//pomocná je Root levý ukazatel
				root->RPtr = node->RPtr;	//nastavení pointerů
				root->LPtr = node->LPtr;
				root->BSTNodeCont= node->BSTNodeCont;	//nastavení nodecont
				root->Key = node->Key;	// nastavení kliče
				free(node);		//uvolnění z paměti
			}
		}
		else if(root ->RPtr != NULL)	//pokud RPtr neni NULL
		{
			if(root->LPtr == NULL)		//a LPtr je NULL
			{
				tBSTNodePtr node = root->RPtr;		//pomocná je ROOT pravý ukazatel
				root->RPtr = node->RPtr;		//nsatavení pointerů
				root->LPtr = node->LPtr;
				root->BSTNodeCont= node->BSTNodeCont;	//nastavení node cont
				root->Key = node->Key;		//nastavení klíče
				free(node);	//uvolnění z paměti
			}
		}
		else if(root->RPtr == NULL)	//Jinak pokud je pravý ukazatel NULL
		{
			if(root->LPtr == NULL)	//a také levý ukazatel je NULL     NULL<-- ROOT --> NULL
			{
				free(*RootPtr);		//uvolnění z mem
				BSTInit(RootPtr);	//(*RootPte) = NULL
			}
		}
		else
		{
			ReplaceByRightmost(root, &(root->LPtr));		// zavolání fce pro přeskládání stromu
			BSTDelete(&(root->LPtr), root->Key);	//znovu zavolání fce pro vymazání listů
		}
	}
} 

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/

	if(*RootPtr == NULL)	//pokud je Root NULL
		return;		//nevadí... return
	tBSTNodePtr root = *RootPtr;	//nastavení rootu na kořen stromu(jeho dereferenci ukazatele)
	BSTDispose(&(root)->RPtr);		// rekurzivní volání doprava
	BSTDispose(&(root)->LPtr);		//rekurzivní volání doleva
	free(*RootPtr);		//uvolnění kořene stromu
	BSTInit(RootPtr);	//(*RootPtr) = NULL
}

/* konec c401.c */

