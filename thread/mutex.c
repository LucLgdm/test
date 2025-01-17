#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct s_mutex{
	pthread_mutex_t	mutex;
	int				data;
}	t_mutex;

void	*add(void *arg)
{
	t_mutex		*mutex = (t_mutex *)arg;
	pthread_t	id = pthread_self();

	for (int i = 0; i < 10; i++){
		pthread_mutex_lock(&mutex->mutex);
		mutex->data++;
		pthread_mutex_unlock(&mutex->mutex);
		printf("thread[%ld], data = %i\n", id, mutex->data);
		sleep(1);
	}
	printf("Fin du thread %ld\n", id);
	pthread_exit(NULL);
}

int	main(void){
	int	nb_thread;
	printf("Combien de threads voulez-vous créer ? ");
	scanf("%d", &nb_thread);

	t_mutex		mutex;
	pthread_t	*thread = malloc(nb_thread * sizeof(pthread_t));

	// Initialisation du mutex
	if (pthread_mutex_init(&mutex.mutex, NULL) != 0)
		exit(EXIT_FAILURE);
	mutex.data = 0;
	printf("mutex.data = 0\n");
	for (int i = 0; i < nb_thread; i++)
	{
		pthread_create(&thread[i], NULL, &add, (void *)&mutex);
		printf("Creation du thread[%ld]\n", thread[i]);
	}
	for (int i = 0; i < nb_thread; i++) pthread_join(thread[i], NULL);

	printf("mutex.data = ?\n%i evidemment !!\n", mutex.data);
	pthread_mutex_destroy(&mutex.mutex);

	for(int i = 0; i < nb_thread; i++) free(thread[i]);
	free(thread);

	return (0);
}
