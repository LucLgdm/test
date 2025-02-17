#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;
    int fd;
    char *args[] = {"/bin/echo", "Hello, World!", NULL};

    // Ouvrir/créer le fichier pour rediriger la sortie
    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    pid = fork();
    if (pid == 0) {
        // Processus enfant : rediriger stdout vers le fichier
        dup2(fd, STDOUT_FILENO);  // Rediriger stdout vers le fichier
        close(fd);  // Fermer après duplication

        if (execve("/bin/echo", args, NULL) == -1) {
            perror("Erreur lors de l'exécution de execve");
            exit(1);
        }
    } else if (pid > 0) {
        // Processus parent : attendre que l'enfant termine
        close(fd);
        wait(NULL);
    } else {
        perror("Erreur lors du fork");
        exit(1);
    }

    return 0;
}
