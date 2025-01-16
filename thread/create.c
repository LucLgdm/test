#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	*thread_1(void *arg){
	printf("Petite attente voulue !\n");
	sleep(2);
	printf("Nous sommes dans le thread 1.\n");
}

void	*thread_2(void *arg){
    int *i = (int *)arg;
    for (int k = 1; k < 15; k++)	*i *= k;
	printf("Nous sommes dans le thread 2.\n");
}

int	main(void)
{
	pthread_t	thread1;
	pthread_t	thread2;
	int			i = 1;

	// On cree les thread avec les fonction thread_1 et thread_2
	pthread_create(&thread1, NULL, thread_1, NULL);
	printf("Thread 1 created.\n");
	printf("Avant passage dans thread : i = %i\n", i);
    pthread_create(&thread2, NULL, thread_2, &i);
	printf("Thread 2 created.\n");

	// On attend l'execution des threads
	pthread_join(thread2, NULL);
	pthread_join(thread1, NULL);
    printf("Apres passage dans thread : i = %i\n", i);
	return (0);
}
