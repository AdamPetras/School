	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;
	L->Last = NULL;
	L->Act = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	//opravdu hnusný a čitelný cyklus ale funguje... :D
	for(tDLElemPtr element = L->First;L->First != NULL;L->First = L->First->rptr)
	{
		free(element);
	}
	DLInitList(L);	// nastavení všech pointerů na NULL
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr element;
	if((element=malloc(sizeof(struct tDLElem)))!= NULL)		//přidělení paměti a ošetření
	{
		element->rptr = L->First;		//pravý od elementa je první
		element->data = val;		//nastavení dat
		element->lptr = NULL;		//levá od elementa je NULL
		if(L->First == NULL)	//pokud je první NULL
		{
			L->Last = element;	//poslední je taky nastaven na elementa
		}
		else
		{
			L->First->lptr = element;	//levý od prvního je nastaven na elementa E <- FL
		}

		L->First = element;		//první je nastaven na element
	}
	else
	DLError();		//pokud se nepovedl malloc error
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr element;
	if((element=malloc(sizeof(struct tDLElem)))!= NULL)		//pokud úspěšně nnaalokujeme prostor
	{
		element->rptr = NULL;	//nastavéme pravý ukazatel na null jelikož insertujeme na poslední místo
		element->lptr = L->Last;	//aktuální poslední nastavíme na předposlední
		element->data = val;		//nastavíme data přidanému elementu
		if(L->Last == NULL)			//pokud je seznam prázdný
		{
			L->First = element;		//první element nastavíme na tento
		}else L->Last->rptr=element;		//jinak pokud je plný tak pravý pointer posledního nastavíme na stávajícího
		L->Last = element;		//ukazatel na posledního přemístíme na aktuálního posledního
	}
	else
		DLError();	//pokud neprojde alokace vyhodí error
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;	//první prvek je nastaven taky jako aktuální
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;	//poslední prvek je nastaven taky jako aktuální
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->First != NULL)	//pokud není prázdný
	{
		*val = L->First->data;		//vložení hodnoty do hodnoty ukazatelu val
	}
	else DLError();
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->Last != NULL)	//pokud není prázdný
		{
			*val = L->Last->data;		//vložení hodnoty do hodnoty ukazatelu val
		}
		else DLError();
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	tDLElemPtr element = L->First;	//nastavení elementa na první prvek
	if(element == NULL)		//ošetření pokud je element NULL
		return;
	if(element == L->Act)	//ošetření pokud je element roven aktuálnímu
		L->Act = NULL;		//nastavení aktuálního prvku na NULL
	if(element == L->Last)	//ošetření pokud je element roven poslednímu
		L->Last = NULL;		//nastavení že poslední je teda NULL
	L->First = L->First->rptr;	//první prvek je pravý prvek prvního prvku
	if(L->First != NULL)		//pokud poslední prvek nenu NULL
		L->First -> lptr = NULL;	//nastavím levý prvek posledního na NULL
	free(element);
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	tDLElemPtr element = L->Last;	//nastavení elementa na poslední prvek
	if(element == NULL)		//ošetření pokud je element NULL
		return;
	if(element == L->Act)	//ošetření pokud je element roven aktuálnímu
		L->Act = NULL;		//nastavení aktuálního prvku na NULL
	if(element == L->Last)	//ošetření pokud je element roven poslednímu
		L->Last = NULL;		//nastavení že poslední je teda NULL
	L->Last = L->Last->lptr;	//první prvek je pravý prvek prvního prvku
	if(L->Last != NULL)		//pokud poslední prvek nenu NULL
		L->Last -> rptr = NULL;	//nastavím pravý prvek posledního na NULL
	free(element);
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if(L->Act == L->Last || L->Act == NULL)		//pokud aktuální je poslední nebo NULL
		return;
	if(L->Act->rptr==NULL)		//pokud pravý od aktuálního je NULL
		return;
	tDLElemPtr element = L->Act->rptr;	//nastavení elementu na pravý aktuální
	L->Act->rptr = element->rptr;	//pravý aktuální je pravý od elementu (o 2 od aktuálního)
	if(element != L->Last)		//pokud neni element prvkem posledním (element= L->Act->rptr)
	{
		element->rptr->lptr = L->Act;	//levý prvek pravého od elementu je nastaven na aktuální
	}
	else
	{
		L->Last = L->Act;		//nastavím poslední prvek za aktuální
	}
	free(element);		//uvolním
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if(L->Act == L->First || L->Act == NULL) //pokud aktuální je první nebo NULL
		return;
	if(L->Act->lptr == NULL)	//pokud je levý prvek aktuálního NULL
		return;
	tDLElemPtr element = L->Act->lptr;		//nastavení elementu na levý prvek aktuálního
	L->Act->lptr = element->lptr;		//nastavení levho prvku aktuálního na levý prvek elementu (o 2 od aktuálního)
	if(element != L->First)		//pokud element není první
	{
		element->lptr->rptr = L->Act;		//pravý prvek levého prvku od elementu je nastaven na aktuální
	}
	else
	{
		L->First = L->Act;	//prvním prvkem je taky aktuálním
	}
	free(element);	//uvolnění
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL)	//pokud je aktuální NULL
		return;
	tDLElemPtr element;
	if((element = malloc(sizeof(struct tDLElem)))!= NULL)		//alokace prostoru pro element a ošetření
	{
		element-> lptr = L->Act;	//levý prvek elementu je aktuální
		element->rptr = L->Act->rptr;	//pravý prvek elementu je pravý prvek aktuálního
		element->data = val;	//nastavení dat z proměnné val
		L->Act->rptr = element;		//pravý prvek aktuálního je element
		if(L->Act != L->Last)		//pokud neni aktuální prvek roven poslednímu
			element ->rptr->lptr = element;		//levý prvek pravého od elementu je element
		else L->Last = element;		//poslední je element
		return;
	}
	DLError();	//error malloc
	return;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL)	//pokud je aktuální NULL
		return;
	tDLElemPtr element;
	if((element = malloc(sizeof(struct tDLElem)))!= NULL)		//rezervování paměti a ošetření
	{
		element->lptr = L->Act->lptr;	//levý prvek elementu je levý prvek aktuálního
		element->rptr = L->Act;		//pravý prvek elementu je aktuální
		element->data = val;		//nastavení dat
		if(L->Act != L->First)	//pokud neni aktuální prvek taky první prvek
			element ->lptr->rptr = element;		//pravý prvek leveho prvku od elementu je element
		else L->First = element;		//(ukazatel na) první prvek je element
		return;
	}
	DLError();	//mallco error
	return;
}
void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if(L->Act!=NULL)		//pokud neni aktuální NULL
		*val = L->Act->data;		//hodnota je hodnota od aktuálního
	else
	{
		DLError();		//aktuální je null zavolám error
		return;
	}
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if(L->Act!=NULL)	//pokud neni aktuální null
		L->Act->data = val;		//data aktuálního jsou val...
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
		if(L->Act==NULL)		//pokud je aktuální NULL
			return;
		if(L->Act!=L->Last)		//pokud aktualní neni poslední
			L->Act = L->Act->rptr;	//přepointerování (aktuální je pravý aktuálního)
		else L->Act = NULL;		//jinak aktuální je NULL
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act==NULL)		//Pokud je aktuální NULL
		return;
	if(L->Act!=L->First)	//pokud neni aktuální prvním
		L->Act = L->Act->lptr;	//ktuální je levý od aktuálního
	else L->Act = NULL;		//jinak aktuální je NULL
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act == NULL)?0:L->Act->data;		//pokud je aktuální NULL vrátím nulu jinak vrátím jeho data
}

/* Konec c206.c*/
