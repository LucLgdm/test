#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


/********************************************************************
 * In this exercice, a thread is created to write in the shared data
 * several thread will be created only to read the shared data.
 * This data will be modified by the writer thread between 2 reads
 ********************************************************************/
typedef struct
{
    int shared_data;
    pthread_mutex_t mutex;
} Data;

void * writer_thread(void* arg){
	Data *data = (Data *)arg;

	// Write 5 time
	for(int i = 0; i < 5; i++){
		pthread_mutex_lock(&data->mutex);
		data->shared_data = rand() % 100;
		printf("Data modified by the writer thread : %d\n", data->shared_data);
		pthread_mutex_unlock(&data->mutex);
	// Simulate the time for writing
		sleep(1);
	}
	return NULL;
}

void * reader_thread(void* arg){
	Data *data = (Data *)arg;
	pthread_t id = pthread_self();

	// Read 5 time
	for(int i = 0; i < 5; i++){
		pthread_mutex_lock(&data->mutex);
		printf("Data read by the reader thread [%ld] : %d\n", id, data->shared_data);
		pthread_mutex_unlock(&data->mutex);
	// Simulate the time for reading
		sleep(1);
	}
	return NULL;
}

int main(){
    Data data;
    int nb_reader = 5;

    // Initialisation
    data.shared_data = 0;
    pthread_mutex_init(&data.mutex, NULL);

    // Creation of the threads
    pthread_t writer, reader[nb_reader];

    pthread_create(&writer, NULL, writer_thread, &data);
    for(int i = 0; i < nb_reader; i++){
        pthread_create(&reader[i], NULL, reader_thread, &data);
    }

    // Wait for the end of the threads
    pthread_join(writer, NULL);
    for(int i = 0; i < nb_reader; i++){
        pthread_join(reader[i], NULL);
    }

    // Destruction
    pthread_mutex_destroy(&data.mutex);

    return 0;
}
