
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, říjen 2017                                        */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;


/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen )
{
	char stackChar;
	stackTop(s,&stackChar);		//sebrání nejvrchnějšího prvku zásobníku
	for(;!stackEmpty(s);)
	{
		if(stackChar!='(')
		{

			stackPop(s);	//popnutí zásobníku
			//(*postLen)++ opravdu hnusná kombinace ale je to pouhá inkrementace délky řetězce
			postExpr[(*postLen)++]=stackChar;	//nastavení na určitem místě (podle postLen) řetězce na znak ze zásobníku
			stackTop(s,&stackChar);	//načtení data(znaku) ze zásobníku do vstupního řetězce
		}
		else
		{
			stackPop(s); //popnutí zásobníku
			return;
		}
	}

}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen )
{
	char stackChar;
	if(!stackEmpty(s))
	{
		//sebrání(ne popnutí) prvního znaku ze zásobníku
		stackTop(s,&stackChar);
	}
	if(stackEmpty(s))
	{
		//pushnutí operátoru na vrch zásobníku pokud je zásobník prázdný
		stackPush(s,c);
	}
	else if(stackChar =='(')
	{
		//pushnutí operátoru na vrch zásobníku pokud je na vrchu levá závorka
		stackPush(s,c);
	}
	else if(stackChar == '+' && c != '-' && c != '+')
	{
		//poshnutí operátoru na vrch zásobníku pokud je na vrchu operátor s nižší prioritou
		stackPush(s,c);
	}
	else if(stackChar == '-' && c != '-' && c != '+')
	{
		//poshnutí operátoru na vrch zásobníku pokud je na vrchu operátor s nižší prioritou
		stackPush(s,c);
	}
	else
	{
		stackPop(s);	//popnutí zásobníku
		//(*postLen)++ opravdu hnusná kombinace ale je to pouhá inkrementace délky řetězce
		postExpr[(*postLen)++] = stackChar;	//nastavení na určitem místě (podle postLen) řetězce na znak ze zásobníku
		doOperation(s, c, postExpr, postLen);
	}

}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr)
{
	//deklarace proměnných
	tStack *s;
	int stringSize;
	int postLen =0;
	char *postExpr;
	//pokud alokace řetězce znaků proběhne správně
	if((postExpr = (char*)malloc(MAX_LEN))!=NULL)
	{
		//pokud alokace zásobníku proběhne správně
		if((s=malloc(sizeof(char*)*STACK_SIZE))!= NULL)
		{
			//zavolání knih fce pro zjištění délky řetězce
			stringSize = snprintf(NULL,0,"%s",infExpr);
			//zavolání fce init zásobníku pro inicializaci
			stackInit(s);
			for(int i =0;i<=stringSize;i++)
			{
				if(infExpr[i]>='0' && infExpr[i]<= '9')
				{
					/*
					 * Načtení čísel
					 * */
					postExpr[postLen++] = infExpr[i];
				}
				else if(infExpr[i] >= 'A' && infExpr[i] <= 'Z')
				{
					/*
					 * Načtení velkých písmen
					 * */
					postExpr[postLen++] = infExpr[i];
				}
				else if(infExpr[i] >= 'a' && infExpr[i] <= 'z')
				{
					/*
					 * Načtení malých pismen
					 * */
					postExpr[postLen++] = infExpr[i];
				}
				else if(infExpr[i] == ')')
				{
					/*
					 * pokud se vyskytne pravá závorka tak zavolám funkci která se postará o
					 * odebrání části položky ze zásobníku dokud nenarazím na levou závorku
					 * */
					untilLeftPar(s, postExpr, &postLen);
				}
				else if(infExpr[i] == '(')
				{
					/*
					 * pokud se vyskytne levá závorka je pushnuta(poslána) na zásobník
					 * */
					stackPush(s,infExpr[i]);
				}
				else if(infExpr[i] == '=')
				{
					/*
					 * Pokud je zpracovaná položka "=" následné se zásobník vyprazdňuje
					 * (od vrcholu) a jsou přidávány do řetězce (na konec) to všechno se děje ve
					 * smyčce dokud neni zásobník prázdný nakonec se přidá konec řetězce a "="
					 * */
					for (;!stackEmpty(s);)
					{
						stackTop(s, &postExpr[postLen++]);
						stackPop(s);
					}
					postExpr[postLen++] = '=';	//načtení rovná se a inkrementace délky expresionu
					postExpr[postLen] = '\0';
				}
				else
				{
					//funkce na zpracování operace
					doOperation(s,infExpr[i],postExpr,&postLen);
				}
			}
			//uvolnění zásobníku z hromady
			free(s);
			return postExpr;
		}
		else
		{
			//uvolnění řetězce z hromady (kvůli nesprávné alokaci zásobníku)
			free(postExpr);
			return NULL;
		}
	}
	return NULL;
}

/* Konec c204.c */
