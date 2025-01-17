#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct data{
    int data_shared;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Data;

void    *thread1_func(void *arg){
    Data *data = (Data *)arg;

	pthread_mutex_lock(&data->mutex);
	while(data->data_shared == 0){
		printf("Thread 1: waiting...\n");
		// Libere le mutex et attend la condition
		pthread_cond_wait(&data->cond, &data->mutex);
	}
	// Une fois la condition validee
	printf("Thread 1: data_shared = %d\n", data->data_shared);
	
	// Liberation du mutex
	pthread_mutex_unlock(&data->mutex);
}

void    *thread2_func(void *arg){
    Data *data = (Data *)arg;
    
	sleep(5);
	pthread_mutex_lock(&data->mutex);
	data->data_shared = 10;
	printf("Thread 2 produit data %d\n", data->data_shared);

	// Signalement au thread 1 et liberation du mutex
	pthread_cond_signal(&data->cond);
	pthread_mutex_unlock(&data->mutex);
}

int main(){
    Data data;
    pthread_t thread1, thread2;

	// Initialisation des variables
	data.data_shared = 0;
	pthread_mutex_init(&data.mutex, NULL);
	pthread_cond_init(&data.cond, NULL);

    // Creation des threads
    pthread_create(&thread1, NULL, thread1_func, &data);
    pthread_create(&thread2, NULL, thread2_func, &data);

    // Attente de l'execution
    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);

    // Destruction du mutex et de la condition
    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond);
}