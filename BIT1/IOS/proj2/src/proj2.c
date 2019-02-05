/**
 * proj2.c
 *
 *  Created on: 18. 4. 2017
 *    Solution: IOS-DU2, příklad
 *      Author: Adam Petráš, FIT
 *    Compiled: GCC 5.4.0 20160609
 *
**/

enum EChyby
{
	SDILENA_PAMET,
	FORK,
	PARAMS,
	OTEVRENI_SOUBORU,
	VYTVORENI_SEMAFORU,
	ODSTRANENI_SEMAFORU,
	PRACE_SEMAFOREM,
	MALLOC
};

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>
#include "process_working.h"
#include <string.h>
#include <ctype.h>

sem_t *semaphore,*semaphore_write,*semaphore_ready,*semaphore_waiting,*semaphore_center,*semaphore_leave,*semaphore_adult_left,*semaphore_child_left,*semaphore_finished,*semaphore_helpful;
int *total_counter=NULL,*counter_adult = NULL,*counter_child = NULL,*counter_child_finished = NULL,*counter_adult_finished = NULL, *center_state = NULL, sharedCounterTotal = 0
		,shared_center_state = 0,sharedCounterChild = 0,sharedCounterChildFinished=0,sharedCounterAdultFinished = 0,sharedCounterAdult = 0;
int A = 0; int C = 0; int AGT =0; int CGT =0; int AWT = 0; int CWT =0;
FILE *f ;
int main(int argc,char **argv)
{

	f = fopen("proj2.out","w");
	fclose(f);
	if ((argc==2) && (strcmp("--help", argv[1])==0)){
	     	 help();exit(EXIT_SUCCESS);
	}
	load_args(argc,argv);
    srandom(time(0));   // initialize random number generator
    setbuf(stdout, NULL);  // for valid printing
    set_resources();
    pid_t pid = fork();
        if (pid == 0)
        {
        	for(int i =0;i<A;i++)
        	{
        		pid_t adultID = fork();
        		// Fork on second child process for generating serfs
        		if(adultID == 0)
        		{
        			adult();
        			delay_rutine(AGT);
        			exit(0);
        		}
        		else if(adultID<0)
        		{
        			error_output(FORK);
        			exit(0);
        		}
        	}
        }
        else if (pid > 0)
        {
        	for(int j = 0 ;j<C;j++)
        	{
				pid_t childID = fork();
				// Fork on second child process for generating serfs
				if(childID == 0)
				{
					child();
					delay_rutine(CGT);

					exit(0);
				}
				else if(childID < 0)
				{
					error_output(FORK);
					exit(0);
				}
				// Wait on generating processes
        	}
        }
        else
        {
        	error_output(FORK);
        }
        waitpid(pid,NULL, 0);
        clean_resources();

    return 0;
}
void child()
{
	//started
	sem_post(semaphore);
	int procc = ++(*counter_child);
	print_state("C",procc,"started");
	if(*counter_child>(*counter_adult-*counter_adult_finished)*3 && *center_state == 3)
	print_waiting("C",procc,*counter_adult,*counter_child);
	sem_wait(semaphore_ready);
	sem_wait(semaphore);

	//enter
	sem_post(semaphore);
	print_state("C",procc,"enter");
	(*center_state)++;
	sem_wait(semaphore);

	//trying to leave, leave
	sem_post(semaphore_leave);
	delay_rutine(CWT);
	print_state("C",procc,"trying to leave");
	print_state("C",procc,"leave");
	(*center_state)--;
	(*counter_child_finished)++;
	sem_post(semaphore_ready);
	sem_wait(semaphore_leave);

	//finish
	sem_wait(semaphore_waiting);
	if(*counter_child_finished==C)
		for(int i = 0;i<A;i++)
		sem_post(semaphore_finished);
	sem_wait(semaphore_adult_left);
	print_state("C",procc,"finished");



	//finished
}
void adult()
{
	//started
	sem_post(semaphore);
	int procc = ++(*counter_adult);
	print_state("A",procc,"started");
	sem_wait(semaphore);

	//enter, center
	sem_post(semaphore);
	print_state("A",procc,"enter");
	for(int i = 0;i<3;i++)		//každý adult má 3 childy v centru
	sem_post(semaphore_ready);
	delay_rutine(AWT);

	//trying to leave, leave
	sem_wait(semaphore);
	sem_post(semaphore_leave);
	print_state("A",procc,"trying to leave");
	print_state("A",procc,"leave");
	(*counter_adult_finished)++;
	sem_wait(semaphore_leave);

	//finish
	sem_post(semaphore);
	if(*counter_adult_finished == A)
		for(int i = 0;i<C;i++)
			sem_post(semaphore_waiting);
	sem_wait(semaphore_finished);
	for(int i = 0;i<C;i++)
		sem_post(semaphore_adult_left);
	print_state("A",procc,"finished");
	sem_wait(semaphore);
}

void delay_rutine(int maxtime)
{
	if(maxtime == 0)
		return;
	int randomTime = random() % maxtime * 1000;
	usleep(randomTime);
}

void load_args(int argc, char **argv)
{
	if (argc!=7){
		error_output(PARAMS);
	  }else{

	    if(isdigit(*argv[1])){		//A parametr
	      A=atoi(argv[1]);
	      if (A<=0){error_output(PARAMS);}
	    }else{
	    	error_output(PARAMS);
	    }
	    if(isdigit(*argv[2])){		//C parametr
	      C=atoi(argv[2]);
	      if (C<=0){error_output(PARAMS);}
	    }else{
	    	error_output(PARAMS);
	    }
	    if(isdigit(*argv[3])){		//AGT parametr
	      AGT=atoi(argv[3]);
	      if (AGT<0 || AGT>5000){error_output(PARAMS);}
	    }else{
	    	error_output(PARAMS);
	    }
	    if(isdigit(*argv[4])){		//CGT parametr
	      CGT=atoi(argv[4]);
	      if (CGT<0 || CGT>5000){error_output(PARAMS);}
	    }else{
	    	error_output(PARAMS);
	    }
	    if(isdigit(*argv[5])){		//AWT parametr
	   	      AWT=atoi(argv[5]);
	   	      if (AWT<0 || AWT>5000){error_output(PARAMS);}
	   	    }else{
	   	    	error_output(PARAMS);
	   	    }
	    if(isdigit(*argv[6])){		//CWT parametr
	   	      CWT=atoi(argv[6]);
	   	      if (CWT<0 || CWT>5000){error_output(PARAMS);}
	   	    }else{
	   	    	error_output(PARAMS);
	   	    }
	  }
}

void error_output(int chyba)
{
	//metoda na tisknutí chyb podle vstupu
	  if(chyba == SDILENA_PAMET)
		  fprintf(stderr, "Chyba pri vytvareni sdilene pameti.\n");
	  else if(chyba == FORK)
		  fprintf(stderr, "Chyba pri vytvareni procesu.\n");
	  else if(chyba == PARAMS)
		  fprintf(stderr, "Spatne zadane argumenty programu.\n");
	  else if(chyba == OTEVRENI_SOUBORU)
		  fprintf(stderr, "Nepodarilo se otevrit soubor pro vypis.\n");
	  else if(chyba == VYTVORENI_SEMAFORU)
		  fprintf(stderr, "Nepovedlo se spravne vytvorit semafor.\n");
	  else if(chyba == ODSTRANENI_SEMAFORU)
		  fprintf(stderr, "Nepodarilo se odstranit semafor.\n");
	  else if(chyba == PRACE_SEMAFOREM)
		  fprintf(stderr, "Chyba pri praci se semaforem.\n");
	  else if(chyba == MALLOC)
		  fprintf(stderr, "Chyba alokace místa v paměti.\n");
	  else
		  fprintf(stderr, "Doslo k chybe programu \n");
	  fprintf(stderr,"Pro zobrazeni napovedy spustte program s parametrem --help\n");
	  exit(EXIT_FAILURE);
}

void help()
{
	printf("Spousteni programu: ./proj2 A C AGT CGT AWT CWT -\n"
		 "A je počet procesů adult; A > 0.\n"
		 "C je počet procesů child; C > 0.\n"
		 "AGT je maximální hodnota doby (v milisekundách), po které je generován nový proces adult; AGT >= 0 && AGT < 5001.\n"
		 "CGT je maximální hodnota doby (v milisekundách), po které je generován nový proces child; CGT >= 0 && CGT < 5001.\n"
		 "AWT je maximální hodnota doby (v milisekundách), po kterou proces adult simuluje činnost v centru; AWT >= 0 && AWT < 5001.\n"
		 "CWT je maximální hodnota doby (v milisekundách), po kterou proces child simuluje činnost v centru; CWT >= 0 && CWT < 5001.\n");
}

void set_resources()
{
	//sdílený čítač na počet výstupů
    if ((sharedCounterTotal = shmget(IPC_PRIVATE, sizeof (int), IPC_CREAT | 0666)) == -1)
    {
        error_output(SDILENA_PAMET);
    }
    //sdílený čítač na počet adult
    if ((sharedCounterAdult = shmget(IPC_PRIVATE, sizeof (int), IPC_CREAT | 0666)) == -1)
        {
            error_output(SDILENA_PAMET);
        }
    //sdílený čítač na počet child
    if ((sharedCounterChild = shmget(IPC_PRIVATE, sizeof (int), IPC_CREAT | 0666)) == -1)
        {
            error_output(SDILENA_PAMET);
        }
    if ((sharedCounterChildFinished = shmget(IPC_PRIVATE, sizeof (int), IPC_CREAT | 0666)) == -1)
            {
                error_output(SDILENA_PAMET);
            }
    if ((sharedCounterAdultFinished = shmget(IPC_PRIVATE, sizeof (int), IPC_CREAT | 0666)) == -1)
                {
                    error_output(SDILENA_PAMET);
                }
    //sdílený čítač na počet childů v centru
    if ((shared_center_state = shmget(IPC_PRIVATE, sizeof (int), IPC_CREAT | 0666)) == -1)
            {
                error_output(SDILENA_PAMET);
            }
    /********************namapování sdílené paměti****************************/
    if ((counter_adult = (int *) shmat(sharedCounterAdult, NULL, 0)) == NULL)
    {
    	error_output(SDILENA_PAMET);
    }
    if ((counter_child = (int *) shmat(sharedCounterChild, NULL, 0)) == NULL)
    {
    	error_output(SDILENA_PAMET);
    }
    if ((counter_child_finished = (int *) shmat(sharedCounterChildFinished, NULL, 0)) == NULL)
        {
        	error_output(SDILENA_PAMET);
        }
    if ((counter_adult_finished = (int *) shmat(sharedCounterAdultFinished, NULL, 0)) == NULL)
            {
            	error_output(SDILENA_PAMET);
            }
    if ((total_counter = (int *) shmat(sharedCounterTotal, NULL, 0)) == NULL)
    {
    	error_output(SDILENA_PAMET);
 	}
    if ((center_state = (int *) shmat(shared_center_state, NULL, 0)) == NULL)
        {
        	error_output(SDILENA_PAMET);
     	}
    /*************************************************************************/
    *counter_adult = 0;
    *counter_child = 0;
    *center_state = 0;
    *total_counter = 1;
    /***************************Inicializace semaforů*************************/
    if ((semaphore = sem_open("/semaphore", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
    {
    	error_output(MALLOC);
    }
    if ((semaphore_ready = sem_open("/semaphore_ready", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
    	error_output(MALLOC);
    }
    if ((semaphore_waiting = sem_open("/semaphore_waiting", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
    	error_output(MALLOC);
  	}
    if ((semaphore_center = sem_open("/semaphore_center", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
 	{
 		error_output(MALLOC);
	}
    if ((semaphore_leave = sem_open("/semaphore_leave", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
	{
     	error_output(MALLOC);
 	}
    if ((semaphore_finished = sem_open("/semaphore_finished", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
    	error_output(MALLOC);
    }
    if ((semaphore_child_left = sem_open("/semaphore_child_left", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
 	{
    	error_output(MALLOC);
	}
    if ((semaphore_adult_left = sem_open("/semaphore_adult_left", O_CREAT | O_EXCL, 0666, 0)) == SEM_FAILED)
    {
    	error_output(MALLOC);
    }
    if ((semaphore_write = sem_open("/semaphore_write", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
        {
        	error_output(MALLOC);
        }
    if ((semaphore_helpful = sem_open("/semaphore_helpful", O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED)
            {
            	error_output(MALLOC);
            }

    /*************************************************************************/
}

void print_state(char * processName,int counter, char* state)
{
	f = fopen("proj2.out","a");
    sem_wait(semaphore_write);
    fprintf(f,"%d\t: %s %d :\t%s\n",(*total_counter)++, processName,counter, state );
    sem_post(semaphore_write);
    fclose(f);
    /*sem_wait(semaphore_write);
    printf("%d\t: %s %d :\t%s\n",(*total_counter)++, processName,counter, state );
    sem_post(semaphore_write);*/
}

void print_waiting(char * processName,int counter, int a, int c)
{
	f = fopen("proj2.out","a");
    sem_wait(semaphore_write);
    fprintf(f,"%d\t: %s %d :\twaiting : %d : %d\n",(*total_counter)++, processName,counter, a,c );
    sem_post(semaphore_write);
    fclose(f);
	/*sem_wait(semaphore_write);
    printf("%d\t: %s %d :\twaiting : %d : %d\n",(*total_counter)++, processName,counter, a,c );
    sem_post(semaphore_write);*/
}

void clean_resources()
{
	/***vyčištění sdílené paměti***/
    shmctl(sharedCounterTotal, IPC_RMID, NULL);
    shmctl(sharedCounterAdult, IPC_RMID, NULL);
    shmctl(sharedCounterChild, IPC_RMID, NULL);
    shmctl(shared_center_state, IPC_RMID, NULL);
    shmctl(sharedCounterChildFinished, IPC_RMID, NULL);
    shmctl(sharedCounterAdultFinished, IPC_RMID, NULL);
    /******************************/
    /******ukončení semaforů*******/
    sem_close(semaphore);
    sem_close(semaphore_ready);
    sem_close(semaphore_waiting);
    sem_close(semaphore_center);
    sem_close(semaphore_leave);
    sem_close(semaphore_finished);
    sem_close(semaphore_child_left);
    sem_close(semaphore_adult_left);
    sem_close(semaphore_write);
    sem_close(semaphore_helpful);
    /******************************/
    /*****odlinkování semaforů*****/
    sem_unlink("/semaphore");
    sem_unlink("/semaphore_ready");
    sem_unlink("/semaphore_waiting");
    sem_unlink("/semaphore_center");
    sem_unlink("/semaphore_leave");
    sem_unlink("/semaphore_finished");
    sem_unlink("/semaphore_child_left");
    sem_unlink("/semaphore_adult_left");
    sem_unlink("/semaphore_write");
    sem_unlink("/semaphore_helpful");
    /******************************/
}
