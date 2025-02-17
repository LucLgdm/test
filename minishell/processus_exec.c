#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char *args[] = {"/bin/ls", "-l", NULL};  // Arguments pour ls
    char *env[] = {NULL};  // Environnement vide pour cet exemple
    
    if (execve("/bin/ls", args, env) == -1) {
        perror("Erreur lors de l'exécution de execve");
        return 1;
    }

    return 0;  // Cette ligne n'est jamais atteinte si execve réussit
}
