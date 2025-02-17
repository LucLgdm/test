#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <termios.h>

int main(void) {
    struct termios term;
    
    // Vérification si la sortie est un terminal
    if (isatty(STDOUT_FILENO)) {
        printf("La sortie standard est un terminal.\n");
    } else {
        printf("La sortie standard n'est pas un terminal.\n");
    }

    // Obtenir le nom du terminal
    char *tty = ttyname(STDIN_FILENO);
    if (tty) {
        printf("Le nom du terminal est : %s\n", tty);
    } else {
        printf("Aucun terminal associé à l'entrée standard.\n");
    }

    // Obtenir les paramètres actuels du terminal
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("Erreur lors de la récupération des paramètres du terminal");
        return 1;
    }

    // Modifier le terminal en mode brut (raw)
    term.c_lflag &= ~(ICANON | ECHO);  // Désactiver le mode canonique et l'écho
    term.c_cc[VMIN] = 1;  // Lecture sur un seul caractère
    term.c_cc[VTIME] = 0;  // Pas de timeout

    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("Erreur lors de la modification des paramètres du terminal");
        return 1;
    }

    printf("Terminal modifié en mode brut. Entrez une touche : ");
    char ch = getchar();  // Lecture d'un seul caractère
    printf("\nVous avez entré : %c\n", ch);

	// Rétablir le mode canonique (restaurer les paramètres du terminal)
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("Erreur lors de la récupération des paramètres du terminal");
        return 1;
    }

    // Rétablir le mode canonique et l'écho
    term.c_lflag |= (ICANON | ECHO);  // Réactiver le mode canonique et l'écho
    term.c_cc[VMIN] = 1;  // Un caractère minimum par lecture
    term.c_cc[VTIME] = 0;  // Pas de timeout

    // Appliquer les nouvelles configurations pour revenir en mode canonique
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("Erreur lors de la restauration des paramètres du terminal");
        return 1;
    }

    printf("Le terminal est maintenant en mode canonique.\n");

    return 0;
}
