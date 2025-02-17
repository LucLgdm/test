#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    char cwd[1024];
	char *args[] = {"/bin/ls", "-l", NULL};
    char *env[] = {NULL};

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("Le répertoire courant est : %s\n", cwd);
    else
        perror("Erreur lors de getcwd");
	
	pid_t pid = fork();
    
	if (pid == 0){
		if (execve("/bin/ls", args, env) == -1) {
			perror("Erreur lors de l'exécution de execve");
			return 1;
		}
	}else if (pid > 0){
		wait(NULL);
		if (chdir("/home/lde-merc/Documents/bibi") == 0)
			printf("Répertoire changé avec succès !\n");
		else
			perror("Erreur lors de chdir");

		if (getcwd(cwd, sizeof(cwd)) != NULL)
			printf("Le répertoire courant est maintenant : %s\n", cwd);
		else
			perror("Erreur lors de getcwd");

		pid_t new_pid = fork();

        if (new_pid == 0) {
            if (execve("/bin/ls", args, env) == -1) {
                perror("Erreur lors de l'exécution de execve");
                return 1;
            }
        } else if (new_pid > 0) {
            wait(NULL);

            char *path = getenv("PATH");
            if (path != NULL)
                printf("La variable PATH est : %s\n", path);
            else
                printf("La variable PATH n'est pas définie.\n");
        } else {
            perror("Erreur lors du fork");
            return 1;
        }

	}else{
		perror("Erreur lors du fork");
        return 1;
	}

    return 0;
}
