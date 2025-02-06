#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

int main() {
    sem_t *sem = sem_open("/test_semaphore", O_CREAT, 0644, 3);

    pid_t pid = fork();

    if (pid == 0) {
        sem_wait(sem);
        printf("Enfant 1 utilise la fourchette\n");
        sleep(2);
        printf("Enfant 1 libère la fourchette\n");
        sem_post(sem);
		return 0;
    } 
    
	pid_t pid2 = fork();
	if (pid2 == 0){
        sem_wait(sem);
        printf("Enfant 2 utilise la fourchette\n");
        sleep(2);
        printf("Enfant 2 libère la fourchette\n");
        sem_post(sem);
		return 0;
	}

	pid_t pid3 = fork();
	if (pid3 == 0){
        sem_wait(sem);
        printf("Enfant 3 utilise la fourchette\n");
        sleep(2);
        printf("Enfant 3 libère la fourchette\n");
        sem_post(sem);
		return 0;
	}

	waitpid(pid, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);

	sem_close(sem);
	sem_unlink("/test_semaphore");
    return 0;
}
