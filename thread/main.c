#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_1(void *arg) {
    int n = 0;

    sleep(10);
    for(int i = 0; i < 15; i++)     n += i;
    printf("n = %i\n", n);
	printf("Nous sommes dans le thread 1.\n");
}

void *thread_2(void *arg) {
	printf("Nous sommes dans le thread 2.\n");
}

int main(){
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, thread_1, NULL);
    printf("Thread 1 created.\n");
    pthread_create(&thread2, NULL, thread_2, NULL);
    printf("Thread 2 created.\n");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    return 0;
}
