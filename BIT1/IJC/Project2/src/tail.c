/*
 * tail.c
 *
 *  Created on: 3. 4. 2017
 *    Solution: IJC-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "tail.h"
#include <string.h>

int tail_read(int num_line,int line_count,FILE * pFile,int isprint)
{
	char line[SIZE];
	int i=0;
	while(fgets(line, SIZE, pFile)!= NULL)
	{
		i++;
		if(isprint)
		if(i>=line_count-num_line+1)
		{
			printf("%s", line);
		}
	}
	return i;
}

void buffer_clean(char * buffer, size_t size)	//vyčištění bufferu
{
	for(unsigned int i = 0;i<size;i++)
	{
		buffer[i]='\0';
	}
}

char **dyn_std(unsigned int *num_lines,size_t size)
{
    char buffer[SIZE];
    buffer_clean(buffer,SIZE);
    char **str;
    int ch;
    *num_lines = 0;
    size_t line = 0;
    size_t len = 0;
    str = (char**)malloc(sizeof(char*)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(stdin)))	//čtení v cyklu
    {
    	if(ch != '\n' && ch != EOF)	//pokud neni eof nebo nový řádek
    	{

    		if(len+1 > SIZE)	//pokud je velikost větší jak size
    		{

    			fprintf(stderr,"Radek je delsi jak 1024 znaku\n");
    			for (int ch = fgetc(stdin); ch != EOF && ch != '\n'; ch = fgetc(stdin)) {
    			        continue;
    			    }
    			buffer_clean(buffer,SIZE);	//vyčištění bufferu
    			len = 0;
    			continue;
    		}
    		buffer[len++]=ch;
    	}
    	if(ch == '\n')	//pokud je nový řádek alokace a kopírování
        {
    		//printf("%s jop\n",buffer);
        	str[line] = malloc(strlen(buffer)+1);
        	strcpy(str[line],buffer);
        	buffer_clean(buffer,len);
        	len = 0;
        	line++;
        }
        if(line==size){	//realokace pokud dosáhneme počtu řádků
            str = (char**)realloc(str, sizeof(char*)*(size+=16));
            if(!str)return str;
        }
    }
    if(strcmp(buffer,"")!=0)	//porovnání pokud je konečný obsah "" tak se přeskočí
    {
    str[line] = malloc(strlen(buffer)+1);
    strcpy(str[line],buffer);
    len = 0;
    line++;
    }
    //printf("%s hoho!%i\n",buffer,line);
    *num_lines = line;

    return (char**)realloc(str, sizeof(char*)*line);
}

int main(int argv, char **argc)
{

	if(argv==2)
	{
		FILE *pFile= fopen (argc[argv-1],"r");
		int num = tail_read(0,1,pFile,0);	//přečtení a zjištění délky
		fseek(pFile,0,SEEK_SET);	//skok na začátek souboru
		tail_read(10,num,pFile,1);	//přečtění a výpis
		fclose(pFile);

	}
	else
	{
		unsigned int n = 10;
		if(argv > 2 && strcmp(argc[argv-2],"-n")==0)	//porovnání jestli je argument -n
		{
			n = atoi(argc[argv-1]);
		}
		unsigned int num_line =0;
		char **adr;
		adr= dyn_std(&num_line,10);	//přečtení a předání pole ukazatelů na char
		//10 - 10
		unsigned int i;

		if(num_line>n)
			i = num_line-n;
		else
			i = 0;
		for(;i<num_line;i++ )	//výpis
		{
				printf("%s\n",adr[i]);
		}
	}
	return EXIT_SUCCESS;
}

