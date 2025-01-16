#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NB_THREAD 2

typedef struct s_mutex{
    pthread_mutex_t mutex;
    int data;
} t_mutex;

void *add(void *arg){
    t_mutex *mutex = (t_mutex *)arg;
    pthread_t id = pthread_self();

    for(int i = 0; i < 10; i++){
        pthread_mutex_lock(&mutex->mutex);
        mutex->data++;
        pthread_mutex_unlock(&mutex->mutex);
        printf("thread[%ld], data = %i\n", id, mutex->data);
        sleep(1);
    }
    printf("Fin du thread\n");
    pthread_exit(NULL);
}


int main(){
    t_mutex mutex;
    pthread_t   thread[NB_THREAD];

    // Initialisation du mutex
    if (pthread_mutex_init(&mutex.mutex, NULL) != 0)
        exit(EXIT_FAILURE);
    mutex.data = 0;
    printf("mutex.data = 0\n");
    for(int i = 0; i < NB_THREAD; i++){
        pthread_create(&thread[i], NULL, &add, (void*)&mutex);
        printf("Creation du thread[%ld]\n", thread[i]);
    }

    for(int i = 0; i < NB_THREAD; i++){
        pthread_join(thread[i], NULL);
    }
    printf("mutex.data = ?\n%i evidemment !!\n", mutex.data);
    pthread_mutex_destroy(&mutex.mutex);
    return 0;
}
