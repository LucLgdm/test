#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_function(void *arg) {
    printf("Thread en cours d'exécution\n");
    sleep(2);
    printf("Thread terminé\n");
    return NULL;
}

int main() {
    pthread_t thread;

    // Création du thread
    if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
        perror("Erreur lors de la création du thread");
        exit(EXIT_FAILURE);
    }

    // Détachement du thread
    if (pthread_detach(thread) != 0) {
        perror("Erreur lors du détachement du thread");
        exit(EXIT_FAILURE);
    }

    printf("Thread détaché, le programme principal continue...\n");

    // Attente pour montrer que le thread continue en arrière-plan
    sleep(3);

    printf("Fin du programme principal\n");
    return 0;
}
