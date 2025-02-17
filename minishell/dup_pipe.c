#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid_ls, pid_grep;
    int pipe_fd[2];
    char *args1[] = {"/bin/ls", "-l", NULL};  // Première commande (ls)
    char *args2[] = {"/bin/grep", "dup", NULL};  // Deuxième commande (grep)

    // Création du pipe
    if (pipe(pipe_fd) == -1) {
        perror("Erreur lors de la création du pipe");
        exit(1);
    }

    // Fork pour exécuter la première commande "ls"
    pid_ls = fork();
    if (pid_ls == 0) {
        // Processus enfant pour "ls"
        close(pipe_fd[0]);  // Fermer le côté lecture du pipe
        dup2(pipe_fd[1], STDOUT_FILENO);  // Rediriger stdout vers le pipe
        close(pipe_fd[1]);  // Fermer après duplication

        if (execve("/bin/ls", args1, NULL) == -1) {
            perror("Erreur lors de l'exécution de execve pour ls");
            exit(1);
        }
    } else if (pid_ls > 0) {
        // Fork pour exécuter la deuxième commande "grep"
        pid_grep = fork();
        if (pid_grep == 0) {
            // Processus enfant pour "grep"
            close(pipe_fd[1]);  // Fermer le côté écriture du pipe
            dup2(pipe_fd[0], STDIN_FILENO);  // Rediriger stdin depuis le pipe
            close(pipe_fd[0]);  // Fermer après duplication

            if (execve("/bin/grep", args2, NULL) == -1) {
                perror("Erreur lors de l'exécution de execve pour grep");
                exit(1);
            }
        } else {
            // Processus parent : attendre la fin des deux enfants
            close(pipe_fd[0]);
            close(pipe_fd[1]);

            // Attente des processus enfants spécifiques
            if (waitpid(pid_ls, NULL, 0) == -1) {
                perror("Erreur lors de l'attente de ls");
            }
            if (waitpid(pid_grep, NULL, 0) == -1) {
                perror("Erreur lors de l'attente de grep");
            }
        }
    } else {
        perror("Erreur lors du fork pour ls");
        exit(1);
    }

    return 0;
}
