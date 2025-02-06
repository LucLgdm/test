#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    sem_t *sem = sem_open("/test_semaphore", O_CREAT, 0644, 1);

    pid_t pid = fork();

    if (pid == 0) {
        printf("Enfant attend la ressource...\n");
        sem_wait(sem);
        printf("Enfant utilise la ressource\n");
        sleep(2);
        printf("Enfant libère la ressource\n");
        sem_post(sem);
    } 
    else {
        printf("Parent attend la ressource...\n");
        sem_wait(sem);
        printf("Parent utilise la ressource\n");
        sleep(2);
        printf("Parent libère la ressource\n");
        sem_post(sem);

        waitpid(pid, NULL, 0);
        sem_close(sem);
        sem_unlink("/test_semaphore");
    }
    return 0;
}
