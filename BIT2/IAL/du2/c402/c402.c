
/* c402.c: ********************************************************************}
{* Téma: Nerekurzivní implementace operací nad BVS 
**                                     Implementace: Petr Přikryl, prosinec 1994
**                                           Úpravy: Petr Přikryl, listopad 1997
**                                                     Petr Přikryl, květen 1998
**			  	                        Převod do jazyka C: Martin Tuček, srpen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                         Úpravy: Karel Masařík, říjen 2013
**                                         Úpravy: Radek Hranický, říjen 2014
**                                         Úpravy: Radek Hranický, listopad 2015
**                                         Úpravy: Radek Hranický, říjen 2016
**
** S využitím dynamického přidělování paměti, implementujte NEREKURZIVNĚ
** následující operace nad binárním vyhledávacím stromem (předpona BT znamená
** Binary Tree a je u identifikátorů uvedena kvůli možné kolizi s ostatními
** příklady):
**
**     BTInit .......... inicializace stromu
**     BTInsert ........ nerekurzivní vložení nového uzlu do stromu
**     BTPreorder ...... nerekurzivní průchod typu pre-order
**     BTInorder ....... nerekurzivní průchod typu in-order
**     BTPostorder ..... nerekurzivní průchod typu post-order
**     BTDisposeTree ... zruš všechny uzly stromu
**
** U všech funkcí, které využívají některý z průchodů stromem, implementujte
** pomocnou funkci pro nalezení nejlevějšího uzlu v podstromu.
**
** Přesné definice typů naleznete v souboru c402.h. Uzel stromu je typu tBTNode,
** ukazatel na něj je typu tBTNodePtr. Jeden uzel obsahuje položku int Cont,
** která současně slouží jako užitečný obsah i jako vyhledávací klíč 
** a ukazatele na levý a pravý podstrom (LPtr a RPtr).
**
** Příklad slouží zejména k procvičení nerekurzivních zápisů algoritmů
** nad stromy. Než začnete tento příklad řešit, prostudujte si důkladně
** principy převodu rekurzivních algoritmů na nerekurzivní. Programování
** je především inženýrská disciplína, kde opětné objevování Ameriky nemá
** místo. Pokud se Vám zdá, že by něco šlo zapsat optimálněji, promyslete
** si všechny detaily Vašeho řešení. Povšimněte si typického umístění akcí
** pro různé typy průchodů. Zamyslete se nad modifikací řešených algoritmů
** například pro výpočet počtu uzlů stromu, počtu listů stromu, výšky stromu
** nebo pro vytvoření zrcadlového obrazu stromu (pouze popřehazování ukazatelů
** bez vytváření nových uzlů a rušení starých).
**
** Při průchodech stromem použijte ke zpracování uzlu funkci BTWorkOut().
** Pro zjednodušení práce máte předem připraveny zásobníky pro hodnoty typu
** bool a tBTNodePtr. Pomocnou funkci BTWorkOut ani funkce pro práci
** s pomocnými zásobníky neupravujte 
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c402.h"
int solved;

void BTWorkOut (tBTNodePtr Ptr)		{
/*   ---------
** Pomocná funkce, kterou budete volat při průchodech stromem pro zpracování
** uzlu určeného ukazatelem Ptr. Tuto funkci neupravujte.
**/
			
	if (Ptr==NULL) 
    printf("Chyba: Funkce BTWorkOut byla volána s NULL argumentem!\n");
  else 
    printf("Výpis hodnoty daného uzlu> %d\n",Ptr->Cont);
}
	
/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu tBTNodePtr. Tyto funkce neupravujte.
**/

void SInitP (tStackP *S)  
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;  
}	

void SPushP (tStackP *S, tBTNodePtr ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{ 
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (S->top==MAXSTACK) 
    printf("Chyba: Došlo k přetečení zásobníku s ukazateli!\n");
  else {  
		S->top++;  
		S->a[S->top]=ptr;
	}
}	

tBTNodePtr STopPopP (tStackP *S)
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		printf("Chyba: Došlo k podtečení zásobníku s ukazateli!\n");
		return(NULL);	
	}	
	else {
		return (S->a[S->top--]);
	}	
}

bool SEmptyP (tStackP *S)
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}	

/* -------------------------------------------------------------------------- */
/*
** Funkce pro zásobník hotnot typu bool. Tyto funkce neupravujte.
*/

void SInitB (tStackB *S) {
/*   ------
** Inicializace zásobníku.
**/

	S->top = 0;  
}	

void SPushB (tStackB *S,bool val) {
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
	if (S->top==MAXSTACK) 
		printf("Chyba: Došlo k přetečení zásobníku pro boolean!\n");
	else {
		S->top++;  
		S->a[S->top]=val;
	}	
}

bool STopPopB (tStackB *S) {
/*   --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
                            /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0) {
		printf("Chyba: Došlo k podtečení zásobníku pro boolean!\n");
		return(NULL);	
	}	
	else {  
		return(S->a[S->top--]); 
	}	
}

bool SEmptyB (tStackB *S) {
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
  return(S->top==0);
}

/* -------------------------------------------------------------------------- */
/*
** Následuje jádro domácí úlohy - funkce, které máte implementovat. 
*/

void BTInit (tBTNodePtr *RootPtr)	{
/*   ------
** Provede inicializaci binárního vyhledávacího stromu.
**
** Inicializaci smí programátor volat pouze před prvním použitím binárního
** stromu, protože neuvolňuje uzly neprázdného stromu (a ani to dělat nemůže,
** protože před inicializací jsou hodnoty nedefinované, tedy libovolné).
** Ke zrušení binárního stromu slouží procedura BTDisposeTree.
**	
** Všimněte si, že zde se poprvé v hlavičce objevuje typ ukazatel na ukazatel,	
** proto je třeba při práci s RootPtr použít dereferenční operátor *.
**/
	(*RootPtr) = NULL;	//inicializace
}

void BTInsert (tBTNodePtr *RootPtr, int Content) {
/*   --------
** Vloží do stromu nový uzel s hodnotou Content.
**
** Z pohledu vkládání chápejte vytvářený strom jako binární vyhledávací strom,
** kde uzly s hodnotou menší než má otec leží v levém podstromu a uzly větší
** leží vpravo. Pokud vkládaný uzel již existuje, neprovádí se nic (daná hodnota
** se ve stromu může vyskytnout nejvýše jednou). Pokud se vytváří nový uzel,
** vzniká vždy jako list stromu. Funkci implementujte nerekurzivně.
**/
	tBTNodePtr root = (*RootPtr);
	if(root != NULL)	//pokud root neni NULL
	{
		//nastavíme tmp na root a iterujeme pokud temp neni NULL a po každé iteraci tempu přiřazujeme
		//pokud temp cont > Content přiřadíme LPtr jinak RPtr
		for (tBTNodePtr temp = root;temp != NULL; temp = temp->Cont>Content ? temp->LPtr : temp->RPtr)
		{
			if(temp->Cont == Content)	//pokud je content roven tak return
				return;
			root = temp;		//kořen nastavíme na temp
		}
		tBTNodePtr newNode = malloc(sizeof(struct tBTNode));	//vytvoříme nový list
		if(newNode != NULL)	//pokud se podaříla alokace
		{
			newNode->Cont = Content;	//nastavíme hodnotu
			newNode->RPtr = NULL;		//pointery
			newNode->LPtr = NULL;		//...
			if(root->Cont < Content)		//pokud je cont < než content
				root->RPtr = newNode;	//nastavíme že list bude napravo od kořene
			else root->LPtr = newNode;	//jinak bude nalevo
		}
		else return;	//nepovedl se malloc
	}
	else		//root je NULL
	{
		tBTNodePtr newNode = malloc(sizeof(struct tBTNode));	//alokace nového listu
		if(newNode != NULL)		//pokud se alokace provedla
		{
			(*RootPtr)= newNode;	//nastavení rootptr na nový list(v tomto případě kořen)
			newNode->Cont = Content;	//nastavení hodnoty listu(kořene)
			newNode->RPtr= NULL;		//nastavení pointerů
			newNode->LPtr = NULL;		//...
		}else return;	//nepovedl se malloc
	}
}

/*                                  PREORDER                                  */

void Leftmost_Preorder (tBTNodePtr ptr, tStackP *Stack)	{
/*   -----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Preorder navštívené uzly zpracujeme voláním funkce BTWorkOut()
** a ukazatele na ně is uložíme do zásobníku.
**/
	for(;ptr != NULL;ptr = ptr ->LPtr)		//iterace dokuj se ptr != NULL poté projedu a nakonec pointer je LPointer
	{
		SPushP(Stack,ptr);	//pushnu na stack
		BTWorkOut(ptr);		//zavolám fci pro výpis
	}
}

void BTPreorder (tBTNodePtr RootPtr)	{
/*   ----------
** Průchod stromem typu preorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Preorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
	tStackP *st = NULL;	//deklarace stacku (muselo se nadefinovat na NULL kvůli warningu)
	SInitP(st);		//init stacku
	Leftmost_Preorder(RootPtr,st);	//zavolání fce na narazení na nejlevější uzel
	for(tBTNodePtr node = STopPopP(st);!SEmptyP(st); node = STopPopP(st))	// nastavení nodu na vrch zásobníku porovnání
		//pokud neni zásobník prázdný a další nastavení nodu na vrchol(hodnotu vrcholu)...
	{
		Leftmost_Preorder(node->RPtr,st);	//získání nejlevjšího prvku pravého prvku
	}
}


/*                                  INORDER                                   */ 

void Leftmost_Inorder(tBTNodePtr ptr, tStackP *Stack)		{
/*   ----------------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Inorder ukládáme ukazatele na všechny navštívené uzly do
** zásobníku. 
**/
	for(;ptr != NULL;ptr = ptr->LPtr);	//iterace přes pointery dokud neni NULL jinak ptr = -> left ptr
		SPushP(Stack,ptr);	//push na stack
}

void BTInorder (tBTNodePtr RootPtr)	{
/*   ---------
** Průchod stromem typu inorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Inorder a zásobníku ukazatelů. Zpracování jednoho uzlu stromu
** realizujte jako volání funkce BTWorkOut(). 
**/
	tStackP *st = NULL;	//deklarace stacku + definice na NULL (warning)
	SInitP(st);		//init stacku
	Leftmost_Inorder(RootPtr,st);	//jde až do levé větve dokud nenarazí na nejlevější
	for(tBTNodePtr node = STopPopP(st);!SEmptyP(st); node = STopPopP(st))	// node je top a dokud neni stack prázdný tak node nastavujeme na top při každé iteraci
	{
		BTWorkOut(node);	//vypíšeme node
		Leftmost_Preorder(node->RPtr,st);	// a jdeme zase do nejlevější části od dalšího uzlu
	}
}

/*                                 POSTORDER                                  */ 

void Leftmost_Postorder (tBTNodePtr ptr, tStackP *StackP, tStackB *StackB) {
/*           --------
** Jde po levě větvi podstromu, dokud nenarazí na jeho nejlevější uzel.
**
** Při průchodu Postorder ukládáme ukazatele na navštívené uzly do zásobníku
** a současně do zásobníku bool hodnot ukládáme informaci, zda byl uzel
** navštíven poprvé a že se tedy ještě nemá zpracovávat. 
**/
	for(;ptr != NULL;ptr = ptr->LPtr)		//iterace přes pointery dokud neni NULL jinak ptr = -> left ptr
	{
		SPushP(StackP,ptr);	//push pointeru na stack
		SPushB(StackB,true);	//push boolean hodnoty na stack je zde explicitně true
	}
}

void BTPostorder (tBTNodePtr RootPtr)	{
/*           -----------
** Průchod stromem typu postorder implementovaný nerekurzivně s využitím funkce
** Leftmost_Postorder, zásobníku ukazatelů a zásobníku hotdnot typu bool.
** Zpracování jednoho uzlu stromu realizujte jako volání funkce BTWorkOut(). 
**/
	tStackP *ptrStack = NULL;		//dekralace stacku definice na NULL
	tStackB *boolStack = NULL;		//dekralace stacku definice na NULL
	SInitP(ptrStack);		//init ptr stacku
	SInitB(boolStack);		//init bool stacku
	Leftmost_Postorder(RootPtr,ptrStack,boolStack);		//zavolá foo postorder
	for(tBTNodePtr node = STopPopP(ptrStack);!SEmptyP(ptrStack);node = STopPopP(ptrStack)) 	//nastavíme node na top kontrola jestli stack neni null a po každé iteraci nastavené node na vrchol
	{
		SPushP(ptrStack,node);	//push ukazatelu na ptr stack
		if(STopPopB(boolStack) == false)	//pokud je top bool stacku false
		{
			STopPopP(ptrStack);		//popnutí topu zásobníku
			BTWorkOut(node);	//výpis hodnoty
		}
		else
		{
			SPushB(boolStack,false);	//pushnutí boolean hodnoty na stack
			Leftmost_Postorder(node->RPtr,ptrStack,boolStack);		//zavolání fce na průchod stromu postorder
		}
	}
}


void BTDisposeTree (tBTNodePtr *RootPtr)	{
/*   -------------
** Zruší všechny uzly stromu a korektně uvolní jimi zabranou paměť.
**
** Funkci implementujte nerekurzivně s využitím zásobníku ukazatelů.
**/
	tStackP *ptrStack = NULL;	//deklarace a definice stacku
	SInitP(ptrStack);		//inicializace stacku
	if(*RootPtr == NULL)	//pokud je kořen NULL => return
		return;
	if(*RootPtr != NULL) 	//pokud neni NULL
	{
		tBTNodePtr node = *RootPtr;	//node je root
		for(tBTNodePtr tmp = node->LPtr;node != NULL || !SEmptyP(ptrStack);node = tmp,tmp = tmp->LPtr) 	//tmp je root ->LPtr a pokud node != NULL
			//!nebo! stack není empty tak nodu přiřadí tmp a tmp je tmp LPtr
		{
			if(node != NULL)	//pokud neni node NULL
			{
				if(node->RPtr != NULL)		//a node RPtr neni NULL
					SPushP(ptrStack,node->RPtr);	//Pushnutí na stack pravého ukazatele
				free(node);	//uvolnění nodu
			}
			if(node == NULL && !SEmptyP(ptrStack))	// pokud je node NULL a stack ne
				node =STopPopP(ptrStack);	//node je vrchol zásobníku...
		}
	}
}

/* konec c402.c */














