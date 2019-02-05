/*
 * ppm.c
 *
 *  Created on: 15. 3. 2017
 *    Solution: IJC-DU1, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 */
#include "ppm.h"
#include <stdlib.h>
#include "error.h"
#include <ctype.h>
int ppm_read_head(FILE *file, int *xsize,int *ysize)
{
  char tmp;
  int  val;
  if (fscanf(file, "P%c\n", &tmp) != 1 || tmp != '6')
	  {
	  	  warning_msg("Soubor je spatneho formatu nebo neni P6");
	  	  return-1;
	  }

  tmp = getc(file);
  //****přeskočení komentáře****
 /* while (tmp == '#')
    {
      do {
	tmp = getc(file);
      } while (tmp != '\n');
      tmp = getc(file);
    }*/
  //****************************
  if (!isdigit(tmp))
	  {
	  	  warning_msg("Nemuzu precist soubor");
	  	  return -1;
	  }
  ungetc(tmp, file);
  if(!fscanf(file, "%d%d%d\n", xsize, ysize, &val))
	  error_msg("Fscanf nemuze nacist hodnoty ze souboru");
  if (val != 255)
	  {
	  	  warning_msg("Spatny format barvy");
	  	  return -1;
	  }
  return 1;
}

struct ppm * ppm_read(const char *filename)
{
  int xsize, ysize, data;
  //**********opravit alokaci dat******************
  unsigned long data_aloc = 1000*1000*3*sizeof(char);

  struct ppm *image = (struct ppm *) malloc(data_aloc + sizeof(struct ppm));
  //***********************************************
  FILE  *file    = fopen(filename, "r");
  if (!image)
	  {
	  	  warning_msg("Nepovedlo se alokovat misto pro novy obrazek");
	  	  fclose(file);
	  	  return NULL;
	  }
  if (!file)
	  {
	  	  warning_msg("Nepovedlo se otevrit soubor");
	  	  free(image);
	  	  return NULL;
	  }
  if(ppm_read_head(file,&xsize,&ysize)==-1)
  {
	  warning_msg("Nepovedlo se precist hlavicku souboru");
	  free(image);
	  fclose(file);
	  return NULL;
  }
  image->xsize = xsize;
  image->ysize = ysize;
  if (!image->data)
	  {
	  	  warning_msg("Nepovedlo se alokovat prostor pro data");
	  	  free(image);
	  	  fclose(file);
	  	  return NULL;
	  }
  data = fread((void *) image->data, 1, (size_t) (xsize * ysize * 3), file);
  if (data != (xsize * ysize * 3))
	  {
	  	  warning_msg("Nemuzu precist data ze souboru");
	  	  free(image);
	  	  fclose(file);
	  	  return NULL;
	  }
  fclose(file);
  return image;
}

int ppm_write(struct ppm *p, const char * filename)
{
	if(!p)
	{
		warning_msg("Ukazatel na strukturu ukazuje na NULL");
		return -1;
	}
	int num;
	int size = p->xsize * p->ysize * 3;
	FILE *file = fopen(filename, "w");
	if (!file)
	{
		free(p);
		warning_msg("Nepovedlo se otevrit soubor");
		return -1;
	}
	fprintf(file, "P6\n%d %d\n%d\n", p->xsize, p->ysize, 255);
	num = fwrite((void *) p->data, 1, (size_t) size, file);
	if (num != size)
	{
		free(p);
		warning_msg("Nemužu zapsat data z obrazku do souboru");
		return -1;
	}
	fclose(file);
	free(p);
	return 1;
}
