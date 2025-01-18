#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*****************************************************************
 * The goal of this simulation is a deadlock : 2 threads T1 & T2
 * trying to attempt 2 ressources R1 & R2. While T1 works with R1
 * it needs R2 but R2 is used by T2. This last one needs R1 to
 * go on. THe situation is blocked, this is a deadlock !
 *****************************************************************/

pthread_mutex_t mutex_1;
pthread_mutex_t mutex_2;

void *thread_1func(void *arg){
	pthread_mutex_lock(&mutex_1);
	printf("thread 1, mutex 1 locked\nSleep\n");
	sleep(5);
	pthread_mutex_lock(&mutex_2);
	printf("thread 1, mutex 2 locked\nSleep\n");
	sleep(5);
	pthread_mutex_unlock(&mutex_2);
	printf("thread 1, mutex 2 unlock\n");
	pthread_mutex_unlock(&mutex_1);
	printf("thread 1, mutex 1 unlock\n");
	return NULL;
}

void *thread_2func(void *arg){
	pthread_mutex_lock(&mutex_2);
	printf("thread 2, mutex 2 locked\nSleep\n");
	sleep(2);
	pthread_mutex_lock(&mutex_1);
	printf("thread 2, mutex 1 locked\nSleep\n");
	sleep(5);
	pthread_mutex_unlock(&mutex_2);
	printf("thread 2, mutex 2 unlock\n");
	pthread_mutex_unlock(&mutex_1);
	printf("thread 2, mutex 1 unlock\n");
	return NULL;
}

int main(){
	pthread_t thread_1, thread_2;

	pthread_mutex_init(&mutex_1, NULL);
	pthread_mutex_init(&mutex_2, NULL);

	pthread_create(&thread_1, NULL, thread_1func, NULL);
	pthread_create(&thread_2, NULL, thread_2func, NULL);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	pthread_mutex_destroy(&mutex_1);
	pthread_mutex_destroy(&mutex_2);
	return 0;
}
