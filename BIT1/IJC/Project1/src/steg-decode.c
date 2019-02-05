/*
 * steg-decode.c
 *
 *  Created on: 20. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "steg-decode.h"
#include <stdio.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv)
{	
	if(argc == 2)
		if(strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0)
		{
			printf("This program find the information which was hidden by steganography.\nArguments: \
			\n- --help(-h) Shows you help for this programe.\n- *path* Enter the path to ppm file.\n\nCreated by Adam Petráš\nVUT FIT BIT1\n");
			exit(0);
		}
	load_message(ppm_read(load_arg(argc,argv)));
	return 0;
}
const char *load_arg(int argc,char **argv)
{
	if(argc!=2)
	{
		error_msg("Zadany spatne argumenty");
	}
	return argv[argc-1];
}
void load_message(struct ppm *data)
{
	if(data!= NULL)
	{
		static int tmpvalue= 0;	//proměnná pro zjištění n-tého bitu v poli prvočísel
		static int y = 0;			//proměnná do které se načítají jednotlivé bity
		static int off_bit=0;		//proměnná pro zjištění offsetu v bajtu (B má 8 bitů)
		//sqrt(303000000) = 17406,...)
		ba_create(array,17450L);	//vytvoření bitového pole
		Eratosthenes(array);	//výpočet prvočísel
		unsigned long i=2;		//začínáme na 2 indexu
		char msg[1000*1000*3];
		int charcounter=0;
		while(i<303000000L-1)
		{
			if(!BA_GET_BIT_(array,i))
			{

				tmpvalue = ba_ppm_get_bit((&data->data[i]), 0);
				ba_ppm_set_bit((&y),off_bit,tmpvalue);
				off_bit++;
				if(off_bit == CHAR_BIT)
				{
					if(y != 0)
					{
						if(isprint(y))
						{
							//printf("%i ",charcounter);
							msg[charcounter]=y;
							charcounter++;
							off_bit = 0;
						}
						else error_msg("Netisknutelny znak");
					}else break;
				}
			}
			i++;
		}
		msg[charcounter]='\0';
		printf("%s",msg);
		free(data);
	}
}

