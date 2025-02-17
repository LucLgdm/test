#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    sem_t *sem = sem_open("/test_semaphore", O_CREAT, 0644, 1);

    pid_t pid = fork();

    if (pid == 0) {
        printf("Enfant 1 attend la ressource...\n");
        sem_wait(sem);
        printf("Enfant 1 utilise la ressource\n");
        sleep(2);
        printf("Enfant 1 libère la ressource\n");
        sem_post(sem);
		return 0;
    } 
    
	pid_t pid2 = fork();
	if (pid2 == 0){
		printf("Enfant 2 attend la ressource...\n");
        sem_wait(sem);
        printf("Enfant 2 utilise la ressource\n");
        sleep(2);
        printf("Enfant 2 libère la ressource\n");
        sem_post(sem);
		return 0;
	}

	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);

	printf("Parent attend la ressource...\n");
	sem_wait(sem);
	printf("Parent utilise la ressource\n");
	sleep(2);
	printf("Parent libère la ressource\n");
	sem_post(sem);

	sem_close(sem);
	sem_unlink("/test_semaphore");
    return 0;
}
