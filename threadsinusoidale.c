#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semaphoresecondaire,semaphoretertiaire;

int compt = 0;
int n,j=0;
void *fthreadsecondaire(void *arg)
{
	while(1){
 		sem_wait(&semaphoresecondaire);
		printf("%d %8d \n",j++,compt);
		 compt++;
		if(compt==n){
			for(int i = 0;i<2*n;i++)
				sem_post(&semaphoretertiaire);
		}
   	
	}
	pthread_exit(NULL);

}

void *fthreadtertiare(void *arg)
{
	
	while(1){
		sem_wait(&semaphoretertiaire);
		printf("%d %8d\n",j++,compt);
		compt--;
		if(compt==-n){
			for(int i = 0;i<2*n;i++)
				sem_post(&semaphoresecondaire);
		}

	}
	pthread_exit(NULL);

}
int main(int argc ,char* argv[])
{
	int resultat;
	 n	=atoi(argv[1]);
	pthread_t th1,th2;
	//initialisation des semaphores
	resultat = sem_init(&semaphoresecondaire,0,n);
	
	if(resultat == -1){
		perror ("Erreur d'initialisation du semaphoresecondaire\n");
		return 0;
	}

	resultat = sem_init(&semaphoretertiaire,0,0);
	
	if(resultat == -1){
		perror ("Erreur d'initialisation du semaphoretertiaire\n");
		return 0;
	}
	
	resultat = pthread_create (&th1,NULL,fthreadsecondaire,(void*)NULL);
	
	if(resultat == -1){
		perror ("Erreur de lancement du threadsecondaire\n");
		return 0;
	}

	resultat = pthread_create (&th2,NULL,fthreadtertiare,(void*)NULL);
	
	if(resultat == -1){
		perror ("Erreur de lancement du threadtertiaire\n");
		return 0;
	}
	pthread_join(th1,NULL);
	pthread_join(th2,NULL);

	sem_destroy(&semaphoresecondaire);
    	sem_destroy(&semaphoretertiaire);
	return 0;
}