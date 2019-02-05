/*
 * io.c
 *
 *  Created on: 4. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "io.h"
#include <ctype.h>
int warr_num;
int get_word(char *s, int max, FILE *f)
{
	if(f != NULL)
	{
		int character = 0;
		int i =0;
		while((character = fgetc(f)) != EOF)
		{
			//pokud se slovo vleze do max velikosti
			if(max > i)
			{
				//pokud neni mezera tak...
				if(!isspace(character))
				{
					s[i] = character;
					i++;
				}
				else
					if(i != 0)
					{
						break;
					}
			}
			else	//slovo je delší jak max znaků
			{
				if(warr_num==0)
				{
					fprintf(stderr,"Slovo je delsi jak %i znaku\n",max);
				}
				while((character = fgetc(f))!= EOF)
				{
					if(!isspace(character))
						continue;
					break;
				}
				warr_num++;
				return i;
			}
			if (ferror(f) != 0)
			{
				fprintf(stderr,"Chyba pri cneni souboru");
			}
		}
		s[i] = 0;
		if(i != 0)
		{
			return i;
		}
	}
	return EOF;
}
